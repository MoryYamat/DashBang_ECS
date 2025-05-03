#include "Game/Actor/FollowCameraActor.h"

#include "Core/Window/WindowManager.h"

#include "Core/ECS/Entity.h"
#include "Core/ECS/Component/PlayerControllerComponent.h"
#include "Core/ECS/Component/FollowCameraComponent.h"

#include "Core/Utils/EntityUtils.h"

#include "Debug/DebugUtils.h"

#include <glm/gtx/rotate_vector.hpp>


#include <cstdint>
#include <iostream>

FollowCameraActor::FollowCameraActor(ECS& ecs)
{
	Entity entity = ecs.createEntity();

	CameraComponent camComp;
	FollowCameraComponent followCamComp;
	TransformComponent CamTransformComp;

	camComp.aspect = WindowManager::GetAspect();
	// initialize camera vectors

	// Get the ID of the target entity to follow and set it to target
	Entity ePlayerActor = EntityUtils::getEntityByName(ecs, "Player");
	if (ePlayerActor != Entity::INVALID)
	{
		followCamComp.targetEntity = ePlayerActor;
		ecs.addComponent(entity, followCamComp);
		TransformComponent& targetTransform = ecs.get<TransformComponent>(ePlayerActor);

		// Relative coordinates
		followCamComp.offset = { 0.0f, 10.0f, 10.0f };
		
		Logic2DTransformComponent& targetLogic2DTransform = ecs.get<Logic2DTransformComponent>(ePlayerActor);

		// Calculating camera position
		float yawDegrees = targetLogic2DTransform.rotation;
		float yawRadians = glm::radians(yawDegrees);

		followCamComp.offset = glm::rotate(followCamComp.offset, yawRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		// Initialize camera position
		CamTransformComp.position = targetTransform.position + followCamComp.offset;

		initializeCameraVectors(camComp, CamTransformComp, targetTransform);
		ecs.addComponent(entity, CamTransformComp);

		ecs.addComponent(entity, camComp);

		
	}
	else
	{
		std::cerr << "[FollowCameraActor.cpp]: Failed to find Player entity by name!" << std::endl;
	}

	DebugUtils::LogPosition("FollowCameraActor.cpp", camComp.front);
}

// Calculates the coordinate data for drawing the camera component from logical data
void FollowCameraActor::initializeCameraVectors(CameraComponent& camComp, TransformComponent& camTransform, TransformComponent& targetTransformComp)
{
	// debugging
	DebugUtils::GeneralLog("FollowCameraActor.cpp", "Initializing camera vectors...");

	camComp.front = glm::normalize(targetTransformComp.position - camTransform.position);
	camComp.right = glm::normalize(glm::cross(camComp.front, camComp.up));
	//camComp.pitch = glm::degrees(glm::asin(camComp.front.y));
	//camComp.yaw = glm::degrees(glm::atan(camComp.front.x, camComp.front.z));
}
