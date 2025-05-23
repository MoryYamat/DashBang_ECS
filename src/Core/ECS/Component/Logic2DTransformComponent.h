// Position data used for logical calculations such as collision detection

#pragma once 

#include <glm/glm.hpp>


struct Logic2DTransformComponent
{
	//position (x, z)
	glm::vec2 positionXZ = {0.0f, 0.0f};

	// direction (radians)
	float rotation = 0.0f;// radians!

	// Front and right vectors
	glm::vec2 front = glm::vec2(0.0f, 1.0f);
	glm::vec2 right = glm::vec2(1.0f, 0.0f);

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
		// 分岐コストは...
		if (glm::length(front) > 0.0001f)
		{
			return std::atan2(front.x, front.y);
		}
		else
		{
			return rotation;
		}
	}

	// FrontベクトルからRightベクトルを得る
	void UpdateRightFromFront()
	{
		right = glm::vec2(front.y, -front.x);
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
		front = glm::normalize(glm::vec2(glm::sin(rotation), glm::cos(rotation)));
		right = glm::vec2(front.y, -front.x);
	}
};