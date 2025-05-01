// Camera Follow system

#pragma once
#include "Core/ECS/EntityManager.h"

// Camera Following System
class CameraFollowSystem
{
public:
	void Update(ECS& ecs, float deltaTime);
};