#include "InitSkillDatabase.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMStates.hpp"

#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include "Game/Combat/Skill/FSM/Effect/Hook/SkillEffectHook.hpp"
#include "Game/Combat/Skill/FSM/Effect/TriggerCondition/OnTransition.hpp"
#include "Game/Combat/Skill/FSM/Effect/Template/SpawnHitBoxEffect.hpp"
#include "Game/Combat/Skill/FSM/Effect/Template/ResetExecutionStateEffect.hpp"

#include "Game/Combat/Skill/FSM/Trigger/ISkillTriggerCondition.hpp"
#include "Game/Combat/Skill/FSM/Trigger/SkillTriggerConditions.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMDefinition.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillTransitionTable.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMConditions.hpp"
#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Skill/MasterData/SkillEntry.hpp"

// Reset
#include "Game/Combat/Skill/FSM/Reset/ResetHookDefinition.hpp"
#include "Game/Combat/Skill/FSM/Reset/Handler/ResetSkillExecutionContext.hpp"
#include "Game/Combat/Skill/FSM/Reset/Handler/ClearEffectExecutionLog.hpp"

#include "Game/Combat/Skill/FSM/Reset/Trigger/OnResetTransition.hpp"

#include "Game/Combat/Skill/FSM/SkillStateTags.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Combat::Skill::Database::SkillResourceInitialization(eNsECS::EntityMgr& ecs)
{
	using namespace Game::Combat::Skill::Database;
	using namespace gNsSkillFSM;
	using namespace gNsSkillFSM::SkillPhase;
	using namespace Game::Combat::Skill::Data;
	using namespace Game::Combat::Skill::FSM::Condition;
	using namespace Game::Combat::Skill::FSM::Effect;
	using namespace Game::Combat::Skill::FSM::Reset;

	auto& db = ecs.createResource<Game::Combat::Skill::Database::SkillDatabase>();

	SkillEntry testSkill;
	testSkill.def.id = 1;
	testSkill.def.name = "TestSkill";

	testSkill.def.castDuration = 0.6f;
	testSkill.def.activeDuration = 1.0f;
	testSkill.def.recoveryDuration = 1.0f;

	testSkill.def.spawnHitArea = gNsSkill::Def::SpawnHitArea{
		.duration = 2.0f,
		.shape = gNsSkillComp::Attack2DShape {
			gNsSkillComp::Circle2DAttack {
				.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
				.radius = 5.0f // 半径
			}},
		.trajectoryParams = gNsSkillData::SkillTrajectory::LinearTrajectoryParams
		{
			.speed = 10.0f
		},
		.syncWithActivePhase = false,
	};

	testSkill.def.cooldown = 1.0f;

	
	
	testSkill.fsm.transitions = {
		{typeid(Casting), typeid(Active), std::make_shared<CastTimeElapsed>()},
		{typeid(Active), typeid(Recovery), std::make_shared<ActiveTimeElapsed>()},
		{typeid(Recovery), typeid(Completed), std::make_shared<RecoveryTimeElapsed>()},

		// Interrupted(中断フラグが立ったらどこからでも)
		{std::nullopt, typeid(Interrupted), std::make_shared<IsInterrupted>()},

		// === 終了状態 から None に戻す === (すべてのリセットは`Completed`/`Interrupted`から行われることを前提とする)
		{typeid(Completed), typeid(None), std::make_shared<AlwaysTrue>()},
		{typeid(Interrupted), typeid(None), std::make_shared<AlwaysTrue>()},

	};
	testSkill.fsm.initialState = typeid(Casting);

	testSkill.fsm.effectHooks =
	{
		SkillEffectHook { 
			std::make_shared<OnTransition>(StateTag::CASTING, StateTag::ACTIVE), 
			std::make_shared<SpawnHitboxEffect>()
		},
	};

	testSkill.triggerCondition = std::make_shared<SkillTriggerCondition_PhaseEquals>(typeid(None));

	// Modifier
	// MovementFSMModifier
	testSkill.def.movementModifiers =
		MovementModifierPerPhase{
			.movementSpeedMultiplier{
				{StateTag::CASTING, 0.3f},
				{StateTag::ACTIVE, 0.0f},
				{StateTag::RECOVERY, 0.7f}
	}};

	testSkill.fsm.resetHooks =
	{
		SkillFSMResetHook
		{
			.handlers = 
			{
				std::make_shared<ClearEffectExecutionLog>(),
				std::make_shared<ResetSkillExecutionContext>()
			},
			.trigger = std::make_shared<OnResetTransition>(StateTag::COMPLETED, StateTag::NONE)
		},
		
		SkillFSMResetHook
		{
			.handlers =
			{
				std::make_shared<ClearEffectExecutionLog>(),
				std::make_shared<ResetSkillExecutionContext>()
			},
			.trigger = std::make_shared<OnResetTransition>(StateTag::INTERRUPTED, StateTag::NONE)
		},
	};

	db.AddSkill(testSkill);

	// TODO :
	// 
	// `SkillStateTags.hpp`を適用し，typeidのハードコードを修正する
	// 
	// **副作用のイベントフック．および自動解決構造の実装**
	// 
	// **直交FSMのテスト**: スキル詠唱中->MovementSpeedを任意%低下させる．それを定義ドリブンで実装する．
	// 
	// **これらの知見を踏まえて，汎用テンプレートインターフェースを実装する**
	// 
}