// Trajectory data

#pragma once

#include <functional>
#include "Core/ECS/Component/Transform2DComponent.h"

#include "Game/SkillSystem/MasterData/SkillDefinition.h"

struct SkillTrajectoryComponent
{
	// 進行中の経過時間
	float elapsedTime = 0.0f;

	// 軌跡の定義(型情報，動作の種類)
	TrajectoryType type = TrajectoryType::None;

	// 実行時の具体的挙動 (Transformを時間で返す)
	std::function<Transform2DComponent(float)> trajectoryFunc;
};