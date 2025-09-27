#include "HitboxLifetimeControlSystem.hpp"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Game/Combat/Skill/Component/SkillOwnerComponent.hpp"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"
#include "Game/ECS/Component/LifetimeComponent.hpp"

#include "Game/Combat/Skill/FSM/SkillStateTags.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"

// 今後 hitboxEffect 自体をFSM化する場合(改良必要になる)
void Game::Combat::Skill::System::HitboxLifetimeControlSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::Database;
	using namespace Game::ECS::Component;
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::FSM::StateModel;

	auto& db = ecs.getResource<SkillDatabase>();

	for (eNsECS::Entity e : ecs.view<
		SkillOwnerComponent,
		LifetimeComponent
	>())
	{
		auto& owner = ecs.get<SkillOwnerComponent>(e);
		auto& lifetime = ecs.get<LifetimeComponent>(e);

		if (!db.Has(owner.skillId)) continue;
		const auto& def = db.Get(owner.skillId).def;

		if (!def.spawnHitArea.has_value()) continue;
		const auto& spawn = def.spawnHitArea.value();

		if (!spawn.syncWithActivePhase) continue;

		if (!ecs.hasComponent<SkillStateComponent>(owner.caster)) continue;
		const auto& state = ecs.get<SkillStateComponent>(owner.caster);

		// stateがActiveじゃなくなったら，
		if (state.current != StateTag::ACTIVE)
		{
			lifetime.elapsedTime = lifetime.totalLifetime;
		}
	}
}