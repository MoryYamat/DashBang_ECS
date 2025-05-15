// Active Skill Component

#pragma once 

#include "Core/ECS/Entity.h"

// Active Skill 使用時に付与されるデータ
// Data granted when using Active Skill
struct ActiveSkillComponent
{
	Entity caster;

	float timeSinceCast = 0.0f;

	// スキルIDなどもあり
};