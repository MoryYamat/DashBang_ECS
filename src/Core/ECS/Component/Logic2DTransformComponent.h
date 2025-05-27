// Position data used for logical calculations such as collision detection

#pragma once 

#include "Config/CanonicalDefaults.h"

#include "Math/Logic/LogicMathUtils.h"

#include <glm/glm.hpp>


struct Logic2DTransformComponent
{
	//position (x, z)
	glm::vec2 positionXZ = {0.0f, 0.0f};

	// direction (radians)
	float rotation = CanonicalDefaults::kCanonicalRotationY;// radians!

	// Front and right vectors
	glm::vec2 front = CanonicalDefaults::kCanonicalFrowardXZ;
	glm::vec2 right = CanonicalDefaults::kCanonicalRightXZ;

	// scale
	glm::vec2 scale = glm::vec2(1.0f);

	// Frontベクトルから回転を得る
	//float GetRotationYFromFrontVector() const
	//{
	//	// 分岐コストは...
	//	if (glm::length(front) > 0.0001f)
	//	{
	//		return glm::degrees(std::atan2(front.x, front.y));
	//	}
	//	else
	//	{
	//		return rotation;
	//	}
	//}

	// Frontベクトルから回転を得る (radians)
	float GetRotationYFromFrontVector() const
	{
		return LogicDirection::CalcYawFromDirection(front);
	}

	// FrontベクトルからRightベクトルを得る
	void UpdateRightFromFront()
	{
		right = LogicDirection::CalcRightFromForward(front);
	}

	// 初期化時のみ　※ゲームループでは使わない
	/*void UpdateDirectionFromRotation()
	{
		float rad = glm::radians(rotation);
		front = glm::normalize(glm::vec2(glm::sin(rad), glm::cos(rad)));
		right = glm::vec2(front.y, -front.x);
	}*/


	// 初期化時のみ　※ゲームループでは使わない
	void UpdateDirectionFromRotation()
	{
		front = glm::normalize(LogicDirection::CalcForwardFromYaw(rotation));
		right = LogicDirection::CalcRightFromYaw(rotation);
	}
};