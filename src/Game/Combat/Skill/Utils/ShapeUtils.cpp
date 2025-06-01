#include "ShapeUtils.h"

#include "Engine/Math/Logic2D/LogicMathUtils.h"

#include "Engine/Debug/DebugUtils.h"

#include <variant>


gNsSkillComp::Attack2DShape Game::Combat::Skill::Utils::ComputeWorldShape(
	const gNsSkillComp::Attack2DShape& localShape
	, const eNsLogic2DComp::Transform2DComponent& transform
)
{
	return std::visit([&] (const auto& shape) -> gNsSkillComp::Attack2DShape
		{
			using T = std::decay_t<decltype(shape)>;

			// std::cout << "[ShapeUtils.cpp]: Transform RotationY: " << transform.rotationY << std::endl;

			if constexpr (std::is_same_v<T, gNsSkillComp::Circle2DAttack>)
			{
				gNsSkillComp::Circle2DAttack result = shape;
				result.center = transform.positionXZ + eNsLogic2DMath::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale;
				result.radius *= transform.scale;
				return gNsSkillComp::Attack2DShape{ result };
			}
			else if constexpr (std::is_same_v<T, gNsSkillComp::Sector2DAttack>)
			{
				gNsSkillComp::Sector2DAttack result = shape;
				// DebugUtils::LogVector_string("ShpeUtils.cpp (shape.direction)", shape.direction);
				result.center = transform.positionXZ + eNsLogic2DMath::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale;

				// ロジック用方向計算
				result.direction = glm::normalize(eNsLogic2DMath::RotateVec2FromZForward(result.direction, transform.rotationY));
				result.radius *= transform.scale;

				// std::cout << "[ShapeUtils.cpp(ComputeWorldShape)] Sector direction " << result.direction.x << result.direction.y << std::endl;

				// DebugUtils::LogVector("[ShapeUtils.cpp(ComputeWorldShape)] Sector world direction ", result.direction);

				return gNsSkillComp::Attack2DShape{ result };
			}
			else if constexpr (std::is_same_v<T, gNsSkillComp::Rectangle2DAttack>)
			{
				gNsSkillComp::Rectangle2DAttack result = shape;
				// DebugUtils::LogVector_string("ShpeUtils.cpp (shape.direction)", shape.direction);
				result.center = transform.positionXZ + eNsLogic2DMath::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale;

				// ロジック用方向計算
				result.direction = glm::normalize(eNsLogic2DMath::RotateVec2FromZForward(result.direction, transform.rotationY));
				result.width *= transform.scale;
				result.height *= transform.scale;

				// std::cout << "[ShapeUtils.cpp(ComputeWorldShape)] Rect direction " << result.direction.x << result.direction.y << std::endl;

				// DebugUtils::LogVector("[ShapeUtils.cpp(ComputeWorldShape)] Rect world direction ", result.direction);

				return gNsSkillComp::Attack2DShape{ result };
			}
			else
			{
				std::cout << "[ShapeUtils.cpp(ComputeWorldShape)] faile to compute world shape\n";
				return gNsSkillComp::Attack2DShape{}; // fallback: empty
			}
		}, localShape.shape);
}