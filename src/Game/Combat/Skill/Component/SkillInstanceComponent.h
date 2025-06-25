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
		Casting,// スキルのキャスト段階
		Active,// スキルのアクティブ段階(攻撃など)
		Recovery,// スキルの硬直
		Completed, // スキルの完了段階(終了状態)
		Interrupted // スキルの中断段階(スタンや死亡など)
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

		// 将来的には，フラグではなく，**段階制御**を導入する(1スキル複数形状に対応するため)
		bool hasSpawned = false;

		uint16_t skillId;

		SkillPhase phase = SkillPhase::Casting;// スキルの状態段階
		std::vector<eNsECS::Entity> spawnedHitAreas; // 生成されたAttack2DAreaへの参照

		std::unordered_map<SkillPhase, PhaseTiming> timings;

		// スキルのライフタイム
		bool isSkillCompleted = false;

		// スタンや死亡などによる中断フラグ
		bool isInterrupted = false;
	};
}

