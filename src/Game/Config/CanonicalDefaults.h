// Canonical form of data (System Default) (Component Default Value)

#pragma once

#include <GLM/glm.hpp>

// システムデフォルトの値
namespace CanonicalDefaults
{
	constexpr float DefaultCameraYaw = -90.0f;
	constexpr float DefaultCameraPitch = 45.0f;

	constexpr glm::vec3 DefaultPlayerScale = glm::vec3(1.0f);
	constexpr glm::vec3 DefaultCameraOffset = glm::vec3(0.0f, 10.0f, -10.0f);


	constexpr glm::vec2 kLocalForwardXZ = glm::vec2(0.0f, -1.0f);
	constexpr glm::vec2 kLocalCenterXZ = glm::vec2(0.0f, 0.0f);
}