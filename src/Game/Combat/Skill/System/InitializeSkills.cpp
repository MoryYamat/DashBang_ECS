#include "InitializeSkills.h"

#include "Common/GameNamespaceDecl.h"


#include "Game/Combat/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Skill/MasterData/SkillDefinition.h"
#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"

#include <iostream>

void Game::Combat::Skill::System::InitializeSkills(eNsECS::EntityMgr& ecs)
{
	// gNsSkillData::SkillDatabase db;
	
	auto& db = ecs.createResource<gNsSkillData::SkillDatabase>();

	gNsSkillData::SkillDefinition slash;
	slash.id = 1;
	slash.name = "Basic Slash";
	slash.shape = gNsSkillComp::Attack2DShape{ gNsSkillComp::Circle2DAttack{CanonicalDefaults::kLocalCenterXZ, 5.0f} };
	slash.duration = 1.0f;
	slash.trajectoryType = gNsSkillData::TrajectoryType::LinearForward;
	slash.trajectoryParams = gNsSkillData::SkillTrajectory::LinearTrajectoryParams
	{
		.speed = 20.0f
	};
	db.AddSkill(slash);

	gNsSkillData::SkillDefinition slash2;
	slash2.id = 2;
	slash2.name = "Power Slash";
	slash2.shape = gNsSkillComp::Attack2DShape{ gNsSkillComp::Sector2DAttack{CanonicalDefaults::kLocalCenterXZ, CanonicalDefaults::kLocalForwardXZ, 1.0f, 10.0f} };// -Z方向が前方
	slash2.duration = 1.0f;

	db.AddSkill(slash2);

	gNsSkillData::SkillDefinition blade;
	blade.id = 3;
	blade.name = "Blade";
	blade.shape = gNsSkillComp::Attack2DShape{ gNsSkillComp::Rectangle2DAttack{glm::vec2(0.0f, 5.0f), CanonicalDefaults::kLocalForwardXZ, 1.0f, 10.0f} };
	blade.duration = 1.0f;
	// スキル奇跡の抽象定義の選択
	blade.trajectoryType = gNsSkillData::TrajectoryType::RotateAroundSelf;
	blade.trajectoryParams = gNsSkillData::SkillTrajectory::RotateTrajectoryParams
	{// 関数定義
		.startAngle = 60.0f,
		.endAngle = -60.0f
	};
	db.AddSkill(blade);


	std::cout << "[InitializeSkills.cpp(skill register)]: Skill definition and registration in database completed successfully\n";
}