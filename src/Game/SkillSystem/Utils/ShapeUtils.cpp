#include "ShapeUtils.h"

#include "Math/Logic/LogicMathUtils.h"

#include "Debug/DebugUtils.h"

#include <variant>


Attack2DShape ShapeUtils::ComputeWorldShape(const Attack2DShape& localShape, const Transform2DComponent& transform)
{
	return std::visit([&] (const auto& shape) -> Attack2DShape
		{
			using T = std::decay_t<decltype(shape)>;

			// std::cout << "[ShapeUtils.cpp]: Transform RotationY: " << transform.rotationY << std::endl;

			if constexpr (std::is_same_v<T, Circle2DAttack>)
			{
				Circle2DAttack result = shape;
				result.center = transform.positionXZ + LogicDirection::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale;
				result.radius *= transform.scale;
				return Attack2DShape{ result };
			}
			else if constexpr (std::is_same_v<T, Sector2DAttack>)
			{
				Sector2DAttack result = shape;
				// DebugUtils::LogVector_string("ShpeUtils.cpp (shape.direction)", shape.direction);
				result.center = transform.positionXZ + LogicDirection::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale;

				// ロジック用方向計算
				result.direction = glm::normalize(LogicDirection::RotateVec2FromZForward(result.direction, transform.rotationY));
				result.radius *= transform.scale;

				// std::cout << "[ShapeUtils.cpp(ComputeWorldShape)] Sector direction " << result.direction.x << result.direction.y << std::endl;

				// DebugUtils::LogVector("[ShapeUtils.cpp(ComputeWorldShape)] Sector world direction ", result.direction);

				return Attack2DShape{ result };
			}
			else if constexpr (std::is_same_v<T, Rectangle2DAttack>)
			{
				Rectangle2DAttack result = shape;
				// DebugUtils::LogVector_string("ShpeUtils.cpp (shape.direction)", shape.direction);
				result.center = transform.positionXZ + LogicDirection::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale;

				// ロジック用方向計算
				result.direction = glm::normalize(LogicDirection::RotateVec2FromZForward(result.direction, transform.rotationY));
				result.width *= transform.scale;
				result.height *= transform.scale;

				// std::cout << "[ShapeUtils.cpp(ComputeWorldShape)] Rect direction " << result.direction.x << result.direction.y << std::endl;

				// DebugUtils::LogVector("[ShapeUtils.cpp(ComputeWorldShape)] Rect world direction ", result.direction);

				return Attack2DShape{ result };
			}
			else
			{
				std::cout << "[ShapeUtils.cpp(ComputeWorldShape)] faile to compute world shape\n";
				return Attack2DShape{}; // fallback: empty
			}
		}, localShape.shape);
}