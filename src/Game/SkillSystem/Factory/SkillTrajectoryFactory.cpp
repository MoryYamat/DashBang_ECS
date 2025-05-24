#include "SkillTrajectoryFactory.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>

#include <variant>

std::function<Transform2DComponent(float)> SkillTrajectoryFactory::Create(
	const SkillDefinition& def,
	const Transform2DComponent& origin)
{
	auto& params = def.trajectoryParams;
	float duration = def.duration;

	return std::visit([=](auto&& p) -> std::function<Transform2DComponent(float)>
		{
			using T = std::decay_t<decltype(p)>;

			if constexpr (std::is_same_v<T, SkillTrajectory::RotateTrajectoryParams>)
			{
				glm::vec2 center = origin.positionXZ;

				return [=](float t)
					{
						float progress = glm::clamp(t / duration, 0.0f, 1.0f);
						float angle = glm::mix(p.startAngle, p.endAngle, progress);
						return Transform2DComponent{ center, angle, 1.0f };
					};
			}
			else if constexpr (std::is_same_v<T, SkillTrajectory::LinearTrajectoryParams>)
			{
				glm::vec2 start = origin.positionXZ;
				float angle = origin.rotationY;
				glm::vec2 dir = glm::rotate(glm::vec2(0, -1), angle);

				return [=](float t)
					{
						float d = glm::min(t, duration);
						return Transform2DComponent{ start + dir * p.speed, angle, 1.0f };
					};
			}
			else
			{
				return [=](float)
					{
						return origin;
					};
			}

		}, params);
}