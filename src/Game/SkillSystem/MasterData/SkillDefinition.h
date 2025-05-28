// skill definition data structure

#pragma once
#include "Game/SkillSystem/Component/Attack2DAreaComponent.h"

#include "Game/SkillSystem/MasterData/SkillTrajectoryData.h"

#include <string>

#include <vector>


// スキル情報定義
struct SkillDefinition
{
	int id = 0;// 一意のID
	std::string name;// 名前

	float castTime = 0.3f;// キャストタイム(発生までの時間)
	float recoveryTime = 0.4f;// スキル
	float duration = 1.0f;// スキル本体の持続時間	
	float cooldown = 1.0f;// スキルの再使用待機時間

	Attack2DShape shape;// 形状定義

	// 軌跡データ
	TrajectoryType trajectoryType;// 軌跡タイプ
	SkillTrajectory::TrajectoryParamsVariant trajectoryParams;// 軌跡パラメータ
};