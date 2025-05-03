// Specify the camera's target
// 
#pragma once

#include <cstdint>

#include "Core/ECS/Entity.h"

#include <glm/glm.hpp>

struct FollowCameraComponent
{
	// target's entity id 
	Entity targetEntity = Entity::INVALID;
	glm::vec3 offset = { 0.0f, 10.0f, -10.0f };
};