// Specify the camera's target
// 
#pragma once

#include <cstdint>

#include <glm/glm.hpp>

struct FollowCameraComponent
{
	// target's entity id 
	uint32_t targetEntity;
	glm::vec3 offset = { 0.0f, 5.0f, -10.0f };
};