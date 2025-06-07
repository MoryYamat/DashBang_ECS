// skill definition data structure

#pragma once
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"

#include <string>

#include <vector>

#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::Data
{
	// スキル情報定義
	struct SkillDefinition
	{
		int id = 0;// 一意のID
		std::string name;// 名前

		float castTime = 0.3f;// キャストタイム(発生までの時間)
		float recoveryTime = 0.4f;// スキル
		float duration = 1.0f;// スキル本体の持続時間	
		float cooldown = 1.0f;// スキルの再使用待機時間

		bool canBeInterruptedDuringCast = true;
		bool canBeInterruptedDuringActive = false;
		bool canBeInterruptedDuringRecovery = true;

		gNsSkillComp::Attack2DShape shape;// 形状定義

		// 軌跡データ
		TrajectoryType trajectoryType = gNsSkillData::TrajectoryType::None;// 軌跡タイプ
		SkillTrajectory::TrajectoryParamsVariant trajectoryParams = gNsSkillData::SkillTrajectory::StaticTrajectory{};// 軌跡パラメータ
	};
}

