// skill definition data structure

#pragma once
#include "Game/SkillSystem/Component/Attack2DAreaComponent.h"

#include "Game/SkillSystem/MasterData/SkillTrajectoryData.h"

#include <string>

#include <vector>



struct SkillDefinition
{
	int id = 0;// 一意のID
	std::string name;// 名前
	float castTime = 1.0f;
	float cooldown = 1.0f;
	float duration = 1.0f;// 
	Attack2DShape shape;// 形状定義

	TrajectoryType trajectoryType;
	SkillTrajectory::TrajectoryParamsVariant trajectoryParams;
};