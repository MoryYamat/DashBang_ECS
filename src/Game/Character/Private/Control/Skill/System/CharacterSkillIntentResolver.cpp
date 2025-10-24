#include "CharacterSkillIntentResolver.hpp"

#include "Game/Character/Private/Control/Skill/Intent/SkillIntentComponent.hpp"
#include "Game/Combat/Private/Skill/Component/SkillSlotAssignmentComponent.h"
#include "Game/Character/Private/State/Component/Action/ActionStateComponent.hpp"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMTransitionRequestComponent.hpp"

#include "Game/Combat/Private/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Private/Skill/Component/SkillExecutionContextComponent.hpp"

// FSM
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMStates.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

// TODO: `SkillExecutionComponent`をアクターが保持する情報にする
// TODO: `SkillExecutionComponent`の生成をFSMHook化する(SkillExecutionSetUpHookなど)
// TODO: TransitionRequestの`Priority`制御の実装
void Game::Character::Control::Skill::UpdateSkillResolverSystem(Engine::ECS::EntityMgr& ecs)
{
	using namespace Game::Character::Control::Skill;
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::Database;
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::FSM::StateModel;
	using namespace Engine::ECS::Component::Logic2D;
	using namespace Game::Combat::Skill::Component;

	namespace Ops = Engine::ECS::Ops;

	auto& db = ecs.getResource<SkillDatabase>();


	for (Engine::ECS::Entity e : ecs.view<
		Logic2DTransformComponent,
		SkillIntentComponent,
		SkillSlotAssignmentComponent,
		SkillStateComponent
		>()
		)
	{
		auto& intent = Ops::Get<SkillIntentComponent>(ecs, e);
		auto& slotAssign = Ops::Get<SkillSlotAssignmentComponent>(ecs,e);
		auto& state = Ops::Get<SkillStateComponent>(ecs, e);
		auto& logic = Ops::Get<Logic2DTransformComponent>(ecs, e);

		// intentがない場合：スキップ
		if (!intent.isActive) continue;

		// 仮の状態判定: より複雑な判定ができるような実装を考える
		// スキル定義から条件を展開する関数を実装する方式にする
		// 抽象条件評価関数を導入して、SkillDef を軸に判定できるようにするのが理想

		for (auto slot : intent.requestedSlots)
		{
			auto it = slotAssign.slotToSkillId.find(slot);
			if (it == slotAssign.slotToSkillId.end()) continue;

			uint32_t skillId = it->second;


			if (!db.Has(skillId)) continue;
			const auto& entry = db.Get(skillId);


			// Skillの硬直キャンセルやチェーンなどは別システムでFSMとintentと組み合わせて判定
			// stateとスキル定義に基づいて，スキルの発動条件を評価	
			if (!canTriggerSkill(state, entry)) continue;


			// skillExecutionComponent生成
			// Engine::ECS::Entity eSkill = ecs.createEntity();
			if (!ecs.hasComponent<SkillExecutionContextComponent>(e))
			{
				Ops::Add<Game::Combat::Skill::Component::SkillExecutionContextComponent>
					(ecs, e, Game::Combat::Skill::Component::SkillExecutionContextComponent{});
				// ecs.addComponent(e, SkillExecutionContextComponent{});
			}
			auto& exec = ecs.get<SkillExecutionContextComponent>(e);

			exec.caster = e;
			exec.skillId = skillId;
			exec.elapsedTime = 0.0f;
			exec.phaseElapsedTime = 0.0f;
			exec.isInterrupted = false;
			// ecs.addComponent(eSkill, exec);

			// 位置・方向を初期化（発動時のキャラのロジック座標をコピー）
			//Transform2DComponent transform;
			//transform.positionXZ = logic.positionXZ;
			//transform.rotationY = logic.GetRotationYFromFrontVector();
			//transform.front = logic.front;
			//transform.right = logic.right;
			//transform.scale = 1.0f;
			// ecs.addComponent(eSkill, transform);


			// skillFSMstate を 定義された初期状態にリクエストする
			if (!ecs.hasComponent<SkillFSMTransitionRequestComponent>(e)) 
			{
				Ops::Add<Game::Combat::Skill::FSM::StateModel::SkillFSMTransitionRequestComponent>(
					ecs, e, Game::Combat::Skill::FSM::StateModel::SkillFSMTransitionRequestComponent{});
				// ecs.addComponent(e, SkillFSMTransitionRequestComponent{});
			}
			auto& reqComp = ecs.get<SkillFSMTransitionRequestComponent>(e);
			reqComp.requests.push_back(SkillFSMTransitionRequest{
				.requestedTo = entry.fsm.initialState,
				.priority = 100 // 高めでも良い、起動直後なので
			});

			// std::cout << "[SkillIntentResolverSystem.cpp]: Skill " << skillId << " transitioned to " << state.current.name() << "\n";

			// std::cout << "[SkillTrigger] Entity " << e.id
			// 	<< " triggered skill " << skillId
			// 	<< " named as " << entry.def.name
			// 	<< " via slot " << static_cast<int>(slot)
			// 	<< std::endl;
		}
	}
}

bool Game::Character::Control::Skill::canTriggerSkill(
	const Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state,
	const Game::Combat::Skill::Data::SkillEntry& entry
)
{
	using namespace Game::Combat::Skill::FSM;

	const auto& def = entry.def;
	const auto& cond = entry.triggerCondition;

	if (!cond) return false;
	return cond->evaluate(state, def);
}

