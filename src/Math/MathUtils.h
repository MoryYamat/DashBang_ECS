// math

#pragma once

#include "Game/Config/CanonicalDefaults.h"

#include <glm/glm.hpp>

namespace MathUtils
{
	inline float DegreesToRadians(float deg) { return glm::radians(deg); }
	inline float RadiansToDegrees(float rad) { return glm::degrees(rad); }


	// 注意: これは XZ平面上で使用される回転です（Z = -Y）
	inline glm::vec2 RotateVec2_XZ(const glm::vec2& v, float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::vec2(
			c * v.x - s * v.y,
			-(s * v.x + c * v.y)
		);
	}

	// 標準的な2D回転（XY空間）
	inline glm::vec2 RotateVec2(const glm::vec2& v, float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::vec2(
			c * v.x - s * v.y,
			s * v.x + c * v.y
		);
	}

	// 回転行列ベース
	inline glm::mat2 RotationMatrix2D(float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::mat2(c, -s, s, c);
	}

	// -Zが前方の場合の回転したベクトルを得る
	inline glm::vec2 ForwardFromRotationY(float radians)
	{
		return RotateVec2_XZ(CanonicalDefaults::kLocalForwardXZ, radians);
	}

	// -Zが前方の場合のFrontベクトルに対するRightベクトルを得る
	inline glm::vec2 RightFromRotationY(float radians)
	{
		return RotateVec2_XZ(CanonicalDefaults::kLocalForwardXZ, radians);
	}
}
