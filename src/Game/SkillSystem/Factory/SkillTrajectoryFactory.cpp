#include "SkillTrajectoryFactory.h"

#include "Math/MathUtils.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>

#include <variant>

std::function<Transform2DComponent(float)> SkillTrajectoryFactory::Create(
	const SkillDefinition& def,
	const Transform2DComponent& origin)
{
	auto& params = def.trajectoryParams;
	float duration = def.duration;

	// 関数を返す
	return std::visit([=](auto&& p) -> std::function<Transform2DComponent(float)>
		{
			using T = std::decay_t<decltype(p)>;

			if constexpr (std::is_same_v<T, SkillTrajectory::RotateTrajectoryParams>)
			{
				glm::vec2 center = origin.positionXZ;

				float startRad = MathUtils::DegreesToRadians(p.startAngle);
				float endRad = MathUtils::DegreesToRadians(p.endAngle);

				// それぞれパラメーターに応じてラムダを生成
				// t: 経過時間
				return [=](float t)
					{
						float progress = glm::clamp(t / duration, 0.0f, 1.0f);// 経過時間を 0.0 - 1.0 に正規化
						float angle = glm::mix(startRad, endRad, progress);// angle: startAngle から endAngle まで線形補間 
						// Transform2DComponent の rotationY にこの角度を設定
						return Transform2DComponent{ 
							.positionXZ = center, 
							.rotationY = origin.rotationY + angle, 
							.scale = 1.0f };
					};
			}
			else if constexpr (std::is_same_v<T, SkillTrajectory::LinearTrajectoryParams>)
			{
				glm::vec2 start = origin.positionXZ;
				float angle = origin.rotationY;
				glm::vec2 dir = origin.GetFrontXZ();

				return [=](float t)
					{
						float d = glm::min(t, duration);
						glm::vec2 offset = dir * p.speed * d;
						return Transform2DComponent{ 
							.positionXZ = start + offset, 
							.rotationY = angle, 
							.scale = 1.0f };
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