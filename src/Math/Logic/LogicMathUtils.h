// 論理用計算関数群


#pragma once
#include "Config/CanonicalDefaults.h"

#include "Debug/DebugUtils.h"

#include <glm/glm.hpp>

#include <iostream>

namespace LogicMathUtils
{
	// ゲームロジック用（+Zが正面） rotationY = 0 -> front = (0, 1) rotationY = pi/2 -> front = (1, 0)
	inline glm::vec2 RotateVec2_FromPositiveZ(const glm::vec2& vec, float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::vec2(
			c * vec.x + s * vec.y,
			-s * vec.x + c * vec.y
		);
	}

	//

	// +Zが前方の場合のradiansだけ回転したベクトルを得る
	inline glm::vec2 GetForwardXZFromRotationY(float radians)
	{
		return RotateVec2_FromPositiveZ(CanonicalDefaults::kLocalForwardXZ, radians);
	}

	// +Zが前方の場合のradinasだけ回転したFrontベクトルに対するRightベクトルを得る
	inline glm::vec2 GetRightXZFromRotationY(float radians)
	{
		return RotateVec2_FromPositiveZ(CanonicalDefaults::kCanonicalFrowardXZ, radians + glm::half_pi<float>());
	}

	// [-pi, pi]を[0, 2pi]に正規化
	inline float NormalizeAngleTo2Pi(float rad)
	{
		float constexpr twoPi = 2.0f * glm::pi<float>();
		float result = std::fmod(rad, twoPi);
		if (result < 0.0f)
			result += twoPi;
		return result;
	}

	// 回転原点を定義
	// 右手系(+Zが正面(0.0°) : ベクトルの回転をラジアンで返す
	inline float GetRotationYFromFrontXZ(glm::vec2 vecXZ)
	{
		// もう少し良い分岐方法を考える
		if (glm::length(vecXZ) > 0.0001f)
		{
			// atan2 -> [-pi, pi] 
			// return std::atan2(vecXZ.y, vecXZ.x);// +X軸から+Z軸
			// return std::atan2(-vecXZ.y, vecXZ.x);// +X軸から-Z軸
			// return std::atan2(vecXZ.y, -vecXZ.x);// -X軸から+Z軸
			// return std::atan2(-vecXZ.y, -vecXZ.x);// -X軸から-Z軸

			// [0, 2pi] に補正
			// return NormalizeAngleTo2Pi(std::atan2(-vecXZ.x, -vecXZ.y));// -Z軸から-X軸
			// return std::atan2(-vecXZ.x, vecXZ.y);// Z軸から-X軸
			// return std::atan2(vecXZ.x, -vecXZ.y);// -Z軸から+X軸
			return NormalizeAngleTo2Pi(std::atan2(vecXZ.x, vecXZ.y));// +Z軸から+X軸
		}
		else
		{
			return CanonicalDefaults::kCanonicalRotationY;
		}
	}



	// 右手系(反時計回りが正)において，あるベクトルに対する右ベクトルを返す
	inline glm::vec2 GetRightXZFromFrontXZ(glm::vec2 front)
	{
		return glm::vec2(-front.y, front.x);
	}
}