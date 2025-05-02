// A camera actor that follows the PlayerCharacter

#pragma once

#include "Core/ECS/EntityManager.h"

class FollowCameraActor
{
public:
	FollowCameraActor(ECS& ecs);

	void Update(ECS& ecs, float deltaTime);
};