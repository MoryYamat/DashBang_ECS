// Skillのライフタイム情報，Caster，IDを保持．
// 

#pragma once 

#include "Engine/ECS/Entity.h"

#include <unordered_map>

#include <cstdint>

#include "Common/EngineNamespaceDecl.h"

namespace Game::Combat::Skill::Component
{
	// スキル段階
	enum class SkillPhase
	{
		Casting,
		Active,
		Recovery,
		Completed
	};

	struct PhaseTiming
	{
		float duration = 0.0f;

		float cancelableStart = 0.0f;
		float cancelableEnd = 0.0f;

		bool isCancelable(float time) const
		{
			return cancelableStart <= time && time <= cancelableEnd;
		}
	};

	// Active Skill 使用時に付与されるデータ
	// Data granted when using Active Skill
	struct SkillInstanceComponent
	{
		eNsECS::Entity caster;

		float timeSinceCast = 0.0f;

		uint16_t skillId;

		SkillPhase phase = SkillPhase::Casting;// スキルの状態段階
		std::vector<eNsECS::Entity> spawnedHitAreas; // 生成されたAttack2DAreaへの参照

		std::unordered_map<SkillPhase, PhaseTiming> timings;
	};
}

