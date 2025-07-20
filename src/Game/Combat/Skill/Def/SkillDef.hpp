// スキル静的定義情報の基盤

// YAGNI
// YAGNI
// YAGNI

// TODO: FSMとSkillDefの統合管理を考える

#pragma once

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"
#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"
#include "Game/ECS/Component/LifetimeComponent.hpp"

#include <vector>

#include <variant>
#include <typeindex>
#include <unordered_map>
#include <string>
#include <optional>

#include <cstdint>

#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::Def
{
	// TODO: SkillDefinitionを完成させる.

	struct SpawnHitArea
	{
		std::optional<float> duration = std::nullopt; // 必要なら明示
		gNsSkillComp::Attack2DShape shape;// 攻撃判定の形状
		gNsSkillData::SkillTrajectory::TrajectoryParamsVariant trajectoryParams; // 軌跡パラメータ
	};
	
	// スキル定義
	struct SkillDef
	{
		// meta data
		uint32_t id = 0; // 一意のID
		std::string name; // スキルの名前

		// phase timing
		float castDuration;
		float activeDuration;
		float recoveryDuration;


		// Event
		std::optional<SpawnHitArea> spawnHitArea; // スキル発動時に生成される攻撃判定の形状と軌跡

		// クールダウン(アクター固有Componentで制御)(FSM側で制御しない(トリガーを抑える))
		float cooldown = 0.0f;
	};
}

