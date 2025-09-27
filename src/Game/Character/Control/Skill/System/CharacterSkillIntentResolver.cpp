#include "CharacterSkillIntentResolver.hpp"

#include "Game/Character/Control/Skill/Intent/SkillIntentComponent.hpp"
#include "Game/Combat/Skill/Component/SkillSlotAssignmentComponent.h"
#include "Game/Character/State/Component/Action/ActionStateComponent.hpp"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMTransitionRequestComponent.hpp"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Skill/Component/SkillExecutionContextComponent.hpp"

// FSM
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMStates.hpp"

// TODO: `SkillExecutionComponent`をアクターが保持する情報にする
// TODO: `SkillExecutionComponent`の生成をFSMHook化する(SkillExecutionSetUpHookなど)
// TODO: TransitionRequestの`Priority`制御の実装
void Game::Character::Control::Skill::UpdateSkillResolverSystem(eNsECS::EntityMgr& ecs)
{
	using namespace Game::Character::Control::Skill;
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::Database;
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::FSM::StateModel;
	using namespace Engine::ECS::Component::Logic2D;
	using namespace Game::Combat::Skill::Component;

	auto& db = ecs.getResource<SkillDatabase>();


	for (eNsECS::Entity e : ecs.view<
		Logic2DTransformComponent,
		SkillIntentComponent,
		SkillSlotAssignmentComponent,
		SkillStateComponent
		>()
		)
	{
		auto& intent = ecs.get<SkillIntentComponent>(e);
		auto& slotAssign = ecs.get<SkillSlotAssignmentComponent>(e);
		auto& state = ecs.get<SkillStateComponent>(e);
		auto& logic = ecs.get<Logic2DTransformComponent>(e);

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
			// eNsECS::Entity eSkill = ecs.createEntity();
			if (!ecs.hasComponent<SkillExecutionContextComponent>(e))
			{
				ecs.addComponent(e, SkillExecutionContextComponent{});
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
			if (!ecs.hasComponent<SkillFSMTransitionRequestComponent>(e)) {
				ecs.addComponent(e, SkillFSMTransitionRequestComponent{});
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
	const gNsSkillFSM::StateModel::SkillStateComponent& state,
	const gNsSkillData::SkillEntry& entry
)
{
	using namespace Game::Combat::Skill::FSM;

	const auto& def = entry.def;
	const auto& cond = entry.triggerCondition;

	if (!cond) return false;
	return cond->evaluate(state, def);
}

// 廃止予定：型ベースFSM実装後
// 効率問題：すぐには問題にならないが、将来的にキャッシュを検討する
// キャラクターのスキルの意図を反映するかしないか判定するシステム
void Game::Character::Control::Skill::UpdateCharacterSkillIntentResovlver(eNsECS::EntityMgr& ecs)
{
	for (eNsECS::Entity e : ecs.view<
		gNsCharacterControlSkill::SkillIntentComponent,
		gNsSkillComp::SkillSlotAssignmentComponent,
		gNsCharacterState::Action::CharacterActionStateComponent>())
	{
		auto& intent = ecs.get<gNsCharacterControlSkill::SkillIntentComponent>(e);
		const auto& state = ecs.get<gNsCharacterState::Action::CharacterActionStateComponent>(e);

		// 仮判定：スキル発動できる状態でなければintent を無効か
		if (state.current == gNsCharaActionState::ActionState::None)
		{
			// 発動可能状態ならintentはそのまま
		}
		else
		{
			// スキル発動可能状態ならintent を無効化
			intent.isActive = false;
			intent.requestedSlots.clear();
		}
	}
}