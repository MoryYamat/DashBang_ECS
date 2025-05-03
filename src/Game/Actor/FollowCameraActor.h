// A camera actor that follows the PlayerCharacter

#pragma once

#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/CameraComponent.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"

class FollowCameraActor
{
public:
	FollowCameraActor(ECS& ecs);


private:
	void initializeCameraVectors(CameraComponent& camComp, TransformComponent& camTransform, TransformComponent& targetTransformComp);
};