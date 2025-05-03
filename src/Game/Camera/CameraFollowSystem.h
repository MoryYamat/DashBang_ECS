// Camera Follow system

#pragma once
#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/CameraComponent.h"
#include "Core/ECS/Component/TransformComponent.h"


// Camera Following System
namespace CameraFollowSystem
{
	void Update(ECS& ecs, float deltaTime);


	void setCameraVectors(CameraComponent& camComp, TransformComponent& camTransform, TransformComponent& targetTransformComp);

};