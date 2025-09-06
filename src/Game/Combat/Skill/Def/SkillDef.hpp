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

	using SkillID = uint32_t;

	struct SpawnHitArea
	{
		std::optional<float> duration = std::nullopt; // 必要なら明示
		gNsSkillComp::Attack2DShape shape;// 攻撃判定の形状
		gNsSkillData::SkillTrajectory::TrajectoryParamsVariant trajectoryParams; // 軌跡パラメータ

		// Activeフェーズに同期して寿命終了するか
		bool syncWithActivePhase = false;
	};

	// SkillPhase ごとにMovementFSMへの作用を定義する
	struct MovementModifierPerPhase
	{
		std::unordered_map<std::type_index, float> movementSpeedMultiplier;// デフォルト移動速度の補正倍率(0.5f->半分に)
	};
	
	// スキル定義
	struct SkillDef
	{
		// meta data
		SkillID id = 0; // 一意のID
		std::string name; // スキルの名前

		// phase timing
		float castDuration;
		float activeDuration;
		float recoveryDuration;


		// Event
		std::optional<SpawnHitArea> spawnHitArea; // スキル発動時に生成される攻撃判定の形状と軌跡

		// MovementFSM Modifier
		std::optional<MovementModifierPerPhase> movementModifiers;

		// クールダウン(アクター固有Componentで制御)(FSM側で制御しない(トリガーを抑える))
		float cooldown = 0.0f;
	};
}

