// Skillのライフタイム情報，Caster，IDを保持．
// 

#pragma once 

#include "Core/ECS/Entity.h"

#include <cstdint>

// スキル段階
enum class SkillPhase
{
	Casting,
	Active,
	Recovery,
	Completed
};

// Active Skill 使用時に付与されるデータ
// Data granted when using Active Skill
struct SkillInstanceComponent
{
	Entity caster;

	float timeSinceCast = 0.0f;

	uint16_t skillId;

	SkillPhase phase = SkillPhase::Casting;// スキルの状態段階

	std::vector<Entity> spawnedHitAreas; // 生成されたAttack2DAreaへの参照
};