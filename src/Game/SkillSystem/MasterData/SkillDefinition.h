// skill definition data structure

#pragma once
#include "Game/SkillSystem/Component/Attack2DAreaComponent.h"
#include <string>

#include <vector>

struct SkillDefinition
{
	int id;
	std::string name;
	float castTime;
	float cooldown;
	float duration;
	Attack2DShape shape;
};