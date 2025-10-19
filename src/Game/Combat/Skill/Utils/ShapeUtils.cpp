#include "ShapeUtils.h"

#include "Engine/Math/Logic2D/LogicMathUtils.h"

#include "Engine/Debug/DebugUtils.h"

#include <variant>


Game::Combat::Skill::Component::Attack2DShape Game::Combat::Skill::Utils::ComputeWorldShape(
	const Game::Combat::Skill::Component::Attack2DShape& localShape
	, const Engine::ECS::Component::Logic2D::Transform2DComponent& transform
)
{
	return std::visit([&] (const auto& shape) -> Game::Combat::Skill::Component::Attack2DShape
		{
			using T = std::decay_t<decltype(shape)>;

			// std::cout << "[ShapeUtils.cpp]: Transform RotationY: " << transform.rotationY << std::endl;

			if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Circle2DAttack>)
			{
				Game::Combat::Skill::Component::Circle2DAttack result = shape;
				// result.center = transform.positionXZ + Engine::Math::Logic2D::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale;
				result.center = Engine::Math::Logic2D::Transform::ApplyLocalOffset(result.center, transform.positionXZ);
				result.radius *= transform.scale;
				return Game::Combat::Skill::Component::Attack2DShape{ result };
			}
			else if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Sector2DAttack>)
			{
				Game::Combat::Skill::Component::Sector2DAttack result = shape;
				// DebugUtils::LogVector_string("ShpeUtils.cpp (shape.direction)", shape.direction);
				// result.center = transform.positionXZ + Engine::Math::Logic2D::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale;
				result.center = Engine::Math::Logic2D::Transform::ApplyLocalOffset(result.center, transform.positionXZ);

				// ロジック用方向計算
				// result.direction = glm::normalize(Engine::Math::Logic2D::RotateVec2FromZForward(result.direction, transform.rotationY));
				result.direction = glm::normalize(Engine::Math::Logic2D::Transform::TransformDirection(result.direction, transform.rotationY));
				result.radius *= transform.scale;

				// std::cout << "[ShapeUtils.cpp(ComputeWorldShape)] Sector direction " << result.direction.x << result.direction.y << std::endl;

				// DebugUtils::LogVector("[ShapeUtils.cpp(ComputeWorldShape)] Sector world direction ", result.direction);

				return Game::Combat::Skill::Component::Attack2DShape{ result };
			}
			else if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Rectangle2DAttack>)
			{
				Game::Combat::Skill::Component::Rectangle2DAttack result = shape;
				// DebugUtils::LogVector_string("ShpeUtils.cpp (shape.direction)", shape.direction);
				// result.center = transform.positionXZ + Engine::Math::Logic2D::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale;
				result.center = Engine::Math::Logic2D::Transform::ApplyLocalOffset(result.center, transform.positionXZ, transform.rotationY);

				// ロジック用方向計算
				// result.direction = glm::normalize(Engine::Math::Logic2D::RotateVec2FromZForward(result.direction, transform.rotationY));
				result.direction = glm::normalize(Engine::Math::Logic2D::Transform::TransformDirection(result.direction, transform.rotationY));
				result.width *= transform.scale;
				result.height *= transform.scale;

				// std::cout << "[ShapeUtils.cpp(ComputeWorldShape)] Rect direction " << result.direction.x << result.direction.y << std::endl;

				// DebugUtils::LogVector("[ShapeUtils.cpp(ComputeWorldShape)] Rect world direction ", result.direction);

				return Game::Combat::Skill::Component::Attack2DShape{ result };
			}
			else
			{
				std::cout << "[ShapeUtils.cpp(ComputeWorldShape)] faile to compute world shape\n";
				return Game::Combat::Skill::Component::Attack2DShape{}; // fallback: empty
			}
		}, localShape.shape);
}