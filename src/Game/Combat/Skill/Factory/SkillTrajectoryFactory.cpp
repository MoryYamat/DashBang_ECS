#include "SkillTrajectoryFactory.h"

#include "Engine/Math/MathUtils.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>

#include <variant>

std::function<eNsLogic2DComp::Transform2DComponent(float)> Game::Combat::Skill::Factory::SkillTrajectoryFactory::Create(
	const gNsSkillData::SkillDefinition& def,
	const eNsLogic2DComp::Transform2DComponent& origin)
{
	auto& params = def.trajectoryParams;
	float duration = def.duration;

	// 関数を返す
	return std::visit([=](auto&& p) -> std::function<eNsLogic2DComp::Transform2DComponent(float)>
		{
			using T = std::decay_t<decltype(p)>;

			// ワールド座標における回転
			if constexpr (std::is_same_v<T, gNsSkillData::SkillTrajectory::RotateTrajectoryParams>)
			{
				glm::vec2 center = origin.positionXZ;

				float startRad = eNsMath::DegreesToRadians(p.startAngle);
				float endRad = eNsMath::DegreesToRadians(p.endAngle);

				// それぞれパラメーターに応じてラムダを生成
				// t: 経過時間
				return [=](float t)
					{
						float progress = glm::clamp(t / duration, 0.0f, 1.0f);// 経過時間を 0.0 - 1.0 に正規化
						float angle = glm::mix(startRad, endRad, progress);// angle: startAngle から endAngle まで線形補間 

						float yaw = origin.rotationY + angle;
						// Transform2DComponent の rotationY にこの角度を設定
						return eNsLogic2DComp::Transform2DComponent{
							.positionXZ = center,
							// .rotationY = origin.rotationY + angle, 
							.rotationY = yaw,
							.scale = 1.0f,
							.front = eNsLogic2DMath::CalcForwardFromYaw(yaw),
							.right = eNsLogic2DMath::CalcRightFromYaw(yaw),
						};
					};
			}
			// ワールド座標における直線移動
			else if constexpr (std::is_same_v<T, gNsSkillData::SkillTrajectory::LinearTrajectoryParams>)
			{
				glm::vec2 start = origin.positionXZ;
				float angle = origin.rotationY;
				glm::vec2 dir = origin.GetFrontXZ();

				return [=](float t)
					{
						float d = glm::min(t, duration);
						glm::vec2 offset = dir * p.speed * d;
						return eNsLogic2DComp::Transform2DComponent{
							.positionXZ = start + offset,
							.rotationY = angle,
							.scale = 1.0f
						};
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