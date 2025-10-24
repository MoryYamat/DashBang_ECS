#include "SkillExecutionLifetimeSystem.hpp"

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMStates.hpp"
#include "Game/Combat/Private/Skill/Component/SkillExecutionContextComponent.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillStateComponent.hpp"

#include "Game/Combat/Private/Skill/FSM/SkillStateTags.hpp"

#include "Engine/ECS/EntityUtils/EntityUtils.h"


// 削除予定：SkillExecutionをcharacterアクターへ付与する方式に変更したため
void Game::Combat::Skill::System::UpdateSkillExecutionLifetimeSystem(Engine::ECS::EntityMgr& ecs)
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::FSM::StateModel;

	for (Engine::ECS::Entity eSkill : ecs.view<SkillExecutionContextComponent>())
	{
		const auto& exec = ecs.get<SkillExecutionContextComponent>(eSkill);
		const auto& caster = exec.caster;

		if (!ecs.isAlive(caster))
		{
			Engine::ECS::EntityUtils::MarkForPendingDestroy(ecs, eSkill);
			continue;
		}

		if (!ecs.hasComponent<SkillStateComponent>(caster)) continue;
		const auto& state = ecs.get<SkillStateComponent>(caster);

		// FSM が Noneに遷移 かつ SkillExecutionComponentを持つエンティティが存在するならば削除
		if (state.current == StateTag::NONE)
		{
			Engine::ECS::EntityUtils::MarkForPendingDestroy(ecs, eSkill);
			std::cout << "[SkillExecutionLifetimeSystem.cpp]: phase complete -> destroy " << eSkill.id << "\n";
		}
	}

}