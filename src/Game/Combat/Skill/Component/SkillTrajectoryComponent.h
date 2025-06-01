// Trajectory data

#pragma once

#include <functional>
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/Combat/Skill/MasterData/SkillDefinition.h"

#include "Common/EngineNamespaceDecl.h"

#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::Component
{
	struct SkillTrajectoryComponent
	{
		// 進行中の経過時間
		float elapsedTime = 0.0f;

		// 軌跡の定義(型情報，動作の種類)
		gNsSkillData::TrajectoryType type = gNsSkillData::TrajectoryType::None;

		// 実行時の具体的挙動 (Transformを時間で返す)
		std::function<eNsLogic2DComp::Transform2DComponent(float)> trajectoryFunc;
	};
}

