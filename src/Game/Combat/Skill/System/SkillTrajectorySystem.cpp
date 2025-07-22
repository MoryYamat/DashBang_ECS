#include "SkillTrajectorySystem.h"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"
#include "Game/Combat/Skill/Factory/SkillTrajectoryFactory.h"
#include "Game/Combat/Skill/Component/SkillTrajectoryComponent.h"

#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"


#include "Game/ECS/Component/LifetimeComponent.hpp"


#include "Engine/Math/MathUtils.h"

#include "Engine/Debug/DebugUtils.h"

#include <glm/glm.hpp>

#include <iostream>
#include <variant>

void Game::Combat::Skill::System::SkillTrajectorySystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::ECS::Component;
	using namespace eNsLogic2DComp;

	for (eNsECS::Entity e : ecs.view<
		Attack2DAreaComponent,
		SkillTrajectoryComponent,
		Transform2DComponent,
		LifetimeComponent>
		())
	{
		auto& trajComp = ecs.get<SkillTrajectoryComponent>(e);
		auto& transform = ecs.get<Transform2DComponent>(e);
		const auto& lifetime = ecs.get<LifetimeComponent>(e);

		float t = lifetime.elapsedTime;

		std::visit([&](const auto& trajParams) 
			{
				using T = std::decay_t<decltype(trajParams)>;
				if constexpr (std::is_same_v<T, StaticTrajectory>)
				{
					// 処理不要：静的
				}
				else if constexpr (std::is_same_v<T, LinearTrajectoryParams>)
				{
					glm::vec2 offset = transform.front * trajParams.speed * deltaTime;
					transform.positionXZ += offset;
				}
				else if constexpr (std::is_same_v<T, RotateTrajectoryParams>)
				{
					float startRad = eNsMath::DegreesToRadians(trajParams.startAngle);
					float endRad = eNsMath::DegreesToRadians(trajParams.endAngle);

					float progress = glm::clamp(lifetime.elapsedTime / lifetime.totalLifetime, 0.0f, 1.0f);
					float yaw = transform.rotationY + glm::mix(startRad, endRad, progress);
					transform.rotationY = yaw;
					transform.front = eNsLogic2DMath::CalcForwardFromYaw(yaw);
					transform.right = eNsLogic2DMath::CalcRightFromYaw(yaw);
				}
				else if constexpr (std::is_same_v<T, CurvedTrajectoryParams>)
				{

				}
			}, trajComp.trajectory);
	}
}