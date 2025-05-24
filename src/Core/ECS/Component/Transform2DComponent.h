// A component that holds information in 2D world space.
// 2Dワールド空間での情報を保持するコンポーネント

#pragma once 

#include "Game/Config/CanonicalDefaults.h"

#include "Math/MathUtils.h"

#include <glm/gtx/rotate_vector.hpp>

#include <GLM/glm.hpp>

struct Transform2DComponent
{
	glm::vec2 positionXZ = { 0.0f, 0.0f };
	float rotationY = 0.0f; // radians
	float scale = 1.0f; // uniform

	// front (radians前提)
	glm::vec2 GetFrontXZ() const
	{
		return MathUtils::ForwardFromRotationY(rotationY);
	}

	// right (radians前提)
	glm::vec2 GetRightXZ() const
	{
		return MathUtils::RightFromRotationY(rotationY);
	}
};