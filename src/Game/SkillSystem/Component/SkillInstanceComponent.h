// Skillのライフタイム情報，Caster，IDを保持．
// 

#pragma once 

#include "Core/ECS/Entity.h"

#include <cstdint>

// Active Skill 使用時に付与されるデータ
// Data granted when using Active Skill
struct SkillInstanceComponent
{
	Entity caster;

	float timeSinceCast = 0.0f;

	uint16_t skillId;

	// スキルIDなどもあり
};