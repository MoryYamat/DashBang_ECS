#include "InitSkillDatabase.hpp"

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMStates.hpp"

#include "Game/Combat/Private/Skill/Def/SkillDef.hpp"

#include "Game/Combat/Private/Skill/FSM/Effect/Hook/SkillEffectHook.hpp"
#include "Game/Combat/Private/Skill/FSM/Effect/TriggerCondition/OnTransition.hpp"
#include "Game/Combat/Private/Skill/FSM/Effect/Template/SpawnHitBoxEffect.hpp"
#include "Game/Combat/Private/Skill/FSM/Effect/Template/ResetExecutionStateEffect.hpp"

#include "Game/Combat/Private/Skill/FSM/Trigger/ISkillTriggerCondition.hpp"
#include "Game/Combat/Private/Skill/FSM/Trigger/SkillTriggerConditions.hpp"

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMDefinition.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillTransitionTable.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMConditions.hpp"
#include "Game/Combat/Private/Skill/MasterData/SkillTrajectoryData.h"

#include "Game/Combat/Private/Skill/Component/Attack2DAreaComponent.h"

#include "Game/Combat/Private/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Private/Skill/MasterData/SkillEntry.hpp"

// Reset
#include "Game/Combat/Private/Skill/FSM/Reset/ResetHookDefinition.hpp"
#include "Game/Combat/Private/Skill/FSM/Reset/Handler/ResetSkillExecutionContext.hpp"
#include "Game/Combat/Private/Skill/FSM/Reset/Handler/ClearEffectExecutionLog.hpp"

#include "Game/Combat/Private/Skill/FSM/Reset/Trigger/OnResetTransition.hpp"

#include "Game/Combat/Private/Skill/FSM/SkillStateTags.hpp"

// cc
#include "Game/Character/Private/FSM/CC/CCStateTags.hpp"



void Game::Combat::Skill::Database::SkillResourceInitialization(Engine::ECS::EntityMgr& ecs)
{
	using namespace Game::Combat::Skill::Database;
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::FSM::SkillPhase;
	using namespace Game::Combat::Skill::Data;
	using namespace Game::Combat::Skill::Def;
	using namespace Game::Combat::Skill::FSM::Condition;
	using namespace Game::Combat::Skill::FSM::Effect;
	using namespace Game::Combat::Skill::FSM::Reset;

	auto& db = ecs.createResource<Game::Combat::Skill::Database::SkillDatabase>();

	SkillEntry testSkill;
	testSkill.def.id = 1;
	testSkill.def.name = "TestSkill";

	testSkill.def.castDuration = 0.0f;
	testSkill.def.activeDuration = 0.5f;
	testSkill.def.recoveryDuration = 0.3f;

	CCSpec ccSpec_STUN{
		.type = Game::Character::FSM::CC::StateTag::STUNNED,
		.priority = 300
	};

	CCSpec ccSpec_KNOCK{
		.type = Game::Character::FSM::CC::StateTag::KNOCKDOWNED,
		.priority = 300
	};

	testSkill.def.cc = ccSpec_STUN;

	testSkill.def.spawnHitArea = Game::Combat::Skill::Def::SpawnHitArea{
		.duration = 2.0f,
		.shape = Game::Combat::Skill::Component::Attack2DShape {
			Game::Combat::Skill::Component::Circle2DAttack {
				.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
				.radius = 5.0f // 半径
			}},
		.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::LinearTrajectoryParams
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

	SkillEntry testAttack;
	testAttack.def.id = 2;
	testAttack.def.name = "testAttack";

	testAttack.def.castDuration = 0.2;
	testAttack.def.activeDuration = 1.0f;
	testAttack.def.recoveryDuration = 0.2f;

	testAttack.def.cc = ccSpec_KNOCK;


	testAttack.def.spawnHitArea = Game::Combat::Skill::Def::SpawnHitArea{
	.duration = 2.0f,
	.shape = Game::Combat::Skill::Component::Attack2DShape {
		Game::Combat::Skill::Component::Rectangle2DAttack {
			.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
			.direction = CanonicalDefaults::kLocalForwardXZ,
			.width = 10.f,
			.height = 2.f
	}},
	.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::LinearTrajectoryParams
	{
		.speed = 10.0f
	},
	.syncWithActivePhase = false,
	};

	testAttack.def.cooldown = 1.0f;



	testAttack.fsm.transitions = {
		{typeid(Casting), typeid(Active), std::make_shared<CastTimeElapsed>()},
		{typeid(Active), typeid(Recovery), std::make_shared<ActiveTimeElapsed>()},
		{typeid(Recovery), typeid(Completed), std::make_shared<RecoveryTimeElapsed>()},

		// Interrupted(中断フラグが立ったらどこからでも)
		{std::nullopt, typeid(Interrupted), std::make_shared<IsInterrupted>()},

		// === 終了状態 から None に戻す === (すべてのリセットは`Completed`/`Interrupted`から行われることを前提とする)
		{typeid(Completed), typeid(None), std::make_shared<AlwaysTrue>()},
		{typeid(Interrupted), typeid(None), std::make_shared<AlwaysTrue>()},

	};
	testAttack.fsm.initialState = typeid(Casting);

	testAttack.fsm.effectHooks =
	{
		SkillEffectHook {
			std::make_shared<OnTransition>(StateTag::CASTING, StateTag::ACTIVE),
			std::make_shared<SpawnHitboxEffect>()
		},
	};

	testAttack.triggerCondition = std::make_shared<SkillTriggerCondition_PhaseEquals>(typeid(None));

	// Modifier
	// MovementFSMModifier
	testAttack.def.movementModifiers =
		MovementModifierPerPhase{
			.movementSpeedMultiplier{
				{StateTag::CASTING, 0.3f},
				{StateTag::ACTIVE, 0.0f},
				{StateTag::RECOVERY, 0.7f}
	} };

	testAttack.fsm.resetHooks =
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

	db.AddSkill(testAttack);
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