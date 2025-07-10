#include "InitSkillFSM.hpp"

#include "Game/Combat/Skill/FSM/Definition/SkillFSMStates.hpp"

#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include "Game/Combat/Skill/FSM/Definition/SkillFSMDefinition.hpp"
#include "Game/Combat/Skill/FSM/Definition/SkillTransitionTable.hpp"
#include "Game/Combat/Skill/FSM/Definition/SkillFSMConditions.hpp"
#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Skill/MasterData/SkillEntry.hpp"


#include "Common/GameNamespaceDecl.h"

void Game::Combat::Skill::FSM::SkillFSMInitialization(eNsECS::EntityMgr& ecs)
{
	using namespace Game::Combat::Skill::Database;
	using namespace gNsSkillFSM;
	using namespace gNsSkillFSM::SkillPhase;

	auto& db = ecs.createResource<SkillDatabase>();

	Game::Combat::Skill::Def::SkillDef testSkill;
	testSkill.id = 1;
	testSkill.name = "TestSkill";

	testSkill.castDuration = 0.3f;
	testSkill.activeDuration = 1.0f;
	testSkill.recoveryDuration = 0.3f;

	testSkill.spawnHitArea = gNsSkill::Def::SpawnHitArea{
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

	testSkill.cooldown = 1.0f;

	Game::Combat::Skill::FSM::SkillFSMDefinition testSkillFSM;
	testSkillFSM.id = testSkill.id;

	testSkillFSM.transitions = {
		{typeid(Casting), typeid(Active), std::make_shared<CastTimeElapsed>()},
		{typeid(Active), typeid(Recovery), std::make_shared<ActiveTimeElapsed>()},
		{typeid(Recovery), typeid(Completed), std::make_shared<RecoveryTimeElapsed>()},

		// Interrupted(中断フラグが立ったらどこからでも)
		{std::nullopt, typeid(Interrupted), std::make_shared<IsInterrupted>()},

		// === 終了状態 から None に戻す ===
		{typeid(Completed), typeid(None), std::make_shared<Always>()},
		{typeid(Interrupted), typeid(None), std::make_shared<Always>()},
	};

	

	db.AddSkill(gNsSkillData::SkillEntry{ .def = testSkill, .fsm = testSkillFSM });

	// TODO :
	// FSMSystem実装
	// トリガー機構のリファクタリング
	// 

	// db.AddSkill(testSkill);
}