#include "SkillTrajectorySystem.h"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"
#include "Game/Combat/Skill/Factory/SkillTrajectoryFactory.h"
#include "Game/Combat/Skill/Component/SkillTrajectoryComponent.h"

#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"


#include "Game/ECS/Component/LifetimeComponent.hpp"


#include "Engine/Math/Private/MathUtils.h"

#include "Engine/Debug/Private/DebugUtils.h"

#include <glm/glm.hpp>

#include <iostream>
#include <variant>

void Game::Combat::Skill::System::SkillTrajectorySystem::Update(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::ECS::Component;
	using namespace Engine::ECS::Component::Logic2D;
	using namespace Game::Combat::Skill::Data::SkillTrajectory;

	for (Engine::ECS::Entity e : ecs.view<
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
					float startRad = Engine::Math::DegreesToRadians(trajParams.startAngle);
					float endRad = Engine::Math::DegreesToRadians(trajParams.endAngle);

					float progress = glm::clamp(lifetime.elapsedTime / lifetime.totalLifetime, 0.0f, 1.0f);
					float yaw = transform.rotationY + glm::mix(startRad, endRad, progress);
					transform.rotationY = yaw;
					transform.front = Engine::Math::Logic2D::CalcForwardFromYaw(yaw);
					transform.right = Engine::Math::Logic2D::CalcRightFromYaw(yaw);
				}
				else if constexpr (std::is_same_v<T, CurvedTrajectoryParams>)
				{

				}
			}, trajComp.trajectory);
	}
}