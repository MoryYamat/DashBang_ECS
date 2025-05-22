// A component that holds information in 2D world space.
// 2Dワールド空間での情報を保持するコンポーネント

#pragma once 

#include <GLM/glm.hpp>

struct Transform2DComponent
{
	glm::vec2 positionXZ = { 0.0f, 0.0f };
	float rotationY = 0.0f; // radians
	float scale = 1.0f; // uniform

	// front (radians前提)
	glm::vec2 GetFrontXZ() const
	{
		return glm::vec2(std::cos(rotationY), std::sin(rotationY)); // ← x = cos, z = sin
	}

	// right (radians前提)
	glm::vec2 GetRightXZ() const
	{
		glm::vec2 front = GetFrontXZ();
		return glm::vec2(front.y, -front.x);
	}
};