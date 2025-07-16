#include "InitSkillDatabase.hpp"

#include "Game/Combat/Skill/FSM/Definition/SkillFSMStates.hpp"

#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include "Game/Combat/Skill/FSM/Condition/ISkillTriggerCondition.hpp"
#include "Game/Combat/Skill/FSM/Condition/SkillTriggerConditions.hpp"

#include "Game/Combat/Skill/FSM/Definition/SkillFSMDefinition.hpp"
#include "Game/Combat/Skill/FSM/Definition/SkillTransitionTable.hpp"
#include "Game/Combat/Skill/FSM/Definition/SkillFSMConditions.hpp"
#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Skill/MasterData/SkillEntry.hpp"


#include "Common/GameNamespaceDecl.h"

void Game::Combat::Skill::Database::SkillResourceInitialization(eNsECS::EntityMgr& ecs)
{
	using namespace Game::Combat::Skill::Database;
	using namespace gNsSkillFSM;
	using namespace gNsSkillFSM::SkillPhase;
	using namespace Game::Combat::Skill::Data;
	using namespace Game::Combat::Skill::FSM::Condition;

	auto& db = ecs.createResource<Game::Combat::Skill::Database::SkillDatabase>();

	SkillEntry testSkill;
	testSkill.def.id = 1;
	testSkill.def.name = "TestSkill";

	testSkill.def.castDuration = 0.3f;
	testSkill.def.activeDuration = 1.0f;
	testSkill.def.recoveryDuration = 0.3f;

	testSkill.def.spawnHitArea = gNsSkill::Def::SpawnHitArea{
		.duration = 2.0f,
		.shape = gNsSkillComp::Attack2DShape {
			gNsSkillComp::Circle2DAttack {
				.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
				.radius = 5.0f // 半径
			}},
		.trajectoryParams = gNsSkillData::SkillTrajectory::LinearTrajectoryParams
		{
			.speed = 3.0f
		}
	};

	testSkill.def.cooldown = 1.0f;

	
	
	testSkill.fsm.transitions = {
		{typeid(Casting), typeid(Active), std::make_shared<CastTimeElapsed>()},
		{typeid(Active), typeid(Recovery), std::make_shared<ActiveTimeElapsed>()},
		{typeid(Recovery), typeid(Completed), std::make_shared<RecoveryTimeElapsed>()},

		// Interrupted(中断フラグが立ったらどこからでも)
		{std::nullopt, typeid(Interrupted), std::make_shared<IsInterrupted>()},

		// === 終了状態 から None に戻す ===
		{typeid(Completed), typeid(None), std::make_shared<Always>()},
		{typeid(Interrupted), typeid(None), std::make_shared<Always>()},
	};
	testSkill.fsm.initialState = typeid(Casting);
	
	testSkill.triggerCondition = std::make_shared<SkillTriggerCondition_PhaseEquals>(typeid(None));

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