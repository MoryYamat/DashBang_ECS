// skill definition data structure

#pragma once
#include "Game/SkillSystem/Component/Attack2DAreaComponent.h"

#include "Game/SkillSystem/MasterData/SkillTrajectoryData.h"

#include <string>

#include <vector>



struct SkillDefinition
{
	int id = 0;// ��ӂ�ID
	std::string name;// ���O
	float castTime = 1.0f;
	float cooldown = 1.0f;
	float duration = 1.0f;// 
	Attack2DShape shape;// �`���`

	TrajectoryType trajectoryType;
	SkillTrajectory::TrajectoryParamsVariant trajectoryParams;
};