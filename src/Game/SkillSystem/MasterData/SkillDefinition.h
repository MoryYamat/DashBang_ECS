// skill definition data structure

#pragma once
#include "Game/SkillSystem/Component/Attack2DAreaComponent.h"

#include "Game/SkillSystem/MasterData/SkillTrajectoryData.h"

#include <string>

#include <vector>



struct SkillDefinition
{
	int id = 0;// ˆêˆÓ‚ÌID
	std::string name;// –¼‘O
	float castTime = 1.0f;
	float cooldown = 1.0f;
	float duration = 1.0f;// 
	Attack2DShape shape;// Œ`ó’è‹`

	TrajectoryType trajectoryType;
	SkillTrajectory::TrajectoryParamsVariant trajectoryParams;
};