// math

#pragma once

#include <glm/glm.hpp>

namespace MathUtils
{
	// 2Dベクトルの回転
	inline glm::vec2 RotateVec2(const glm::vec2& v, float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::vec2(
			c * v.x - s * v.y,
			-(s * v.x + c * v.y)
		);
	}

	// 回転行列ベース
	inline glm::mat2 RotationMatrix2D(float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::mat2(c, -s, s, c);
	}
}
