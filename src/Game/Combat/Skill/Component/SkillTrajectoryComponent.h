// Trajectory data

#pragma once

#include <functional>
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/Combat/Skill/MasterData/SkillDefinition.h"

#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"

#include <variant>





namespace Game::Combat::Skill::Component
{
	
	struct SkillTrajectoryComponent
	{
		//// 進行中の経過時間
		//float elapsedTime = 0.0f;

		//// 軌跡の定義(型情報，動作の種類)
		//Game::Combat::Skill::Data::TrajectoryType type = Game::Combat::Skill::Data::TrajectoryType::None;

		using TrajectoryVariant = std::variant<
			std::monostate, 
			Game::Combat::Skill::Data::SkillTrajectory::StaticTrajectory, 
			Game::Combat::Skill::Data::SkillTrajectory::RotateTrajectoryParams, 
			Game::Combat::Skill::Data::SkillTrajectory::LinearTrajectoryParams, 
			Game::Combat::Skill::Data::SkillTrajectory::CurvedTrajectoryParams
		>;

		TrajectoryVariant trajectory = Game::Combat::Skill::Data::SkillTrajectory::StaticTrajectory{};

		// 実行時の具体的挙動 (Transformを時間で返す)
		// std::function<Engine::ECS::Component::Logic2D::Transform2DComponent(float)> trajectoryFunc;
	};
}

