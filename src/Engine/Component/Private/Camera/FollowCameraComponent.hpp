#pragma once

#include "Engine/World/Public/WorldFwd.hpp"

namespace Engine::Component
{
	struct FollowCameraComponent
	{
		// target's entity id 
		Entity targetEntity = Entity::INVALID;
		glm::vec3 offset = { 0.0f, 10.0f, -10.0f };
	};
}