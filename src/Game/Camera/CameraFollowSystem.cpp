#include "CameraFollowSystem.h"

#include "Core/ECS/Component/CameraComponent.h"



#include "Debug/DebugUtils.h"

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

void CameraFollowSystem::Update(ECS& ecs, float deltaTime)
{
	for (Entity e : ecs.view<FollowCameraComponent, TransformComponent, CameraComponent>())
	{


		auto& followCam = ecs.get<FollowCameraComponent>(e);
		auto& camTransform = ecs.get<TransformComponent>(e);
		auto& camComp = ecs.get<CameraComponent>(e);



		// If the targetEntity does not have a TransformComponent
		if (!ecs.hasComponent<TransformComponent>(followCam.targetEntity)) return;

		const auto& targetLogic2DTransform = ecs.get<Logic2DTransformComponent>(followCam.targetEntity);
		TransformComponent targetTransformComp = ecs.get<TransformComponent>(followCam.targetEntity);


		camTransform.position = targetTransformComp.position + followCam.offset;

		// 
		setCameraVectors(camComp, camTransform, targetTransformComp);


		//camTransform.position = targetTransform.position + rotatedOffset;
		// 追従(オフセット)
		//camTransform.position = targetTransform.position + followCam.offset;

		// 方向を追従対象へ
		//glm::vec3 direction = glm::normalize(targetTransform.position - camTransform.position);


		// position log for debbugging
		//DebugUtils::LogPosition("CameraFollowSystem.cpp", camTransform.position);
		//DebugUtils::LogPosition("CameraFollowSystem", camComp.front);

		break;
	}
}

void CameraFollowSystem::setFollowCameraGetFront(FollowCameraComponent& followCamComp, Logic2DTransformComponent& targetLogic2DTransform)
{
	// Calculating camera position
	// 論理データのfrontに対して、対角上に存在するように計算している
	float yawDegrees = targetLogic2DTransform.rotation;
	float yawRadians = glm::radians(yawDegrees);


	followCamComp.offset = glm::rotate(followCamComp.offset, yawRadians, glm::vec3(0.0f, 1.0f, 0.0f));
}

void CameraFollowSystem::setCameraVectors(CameraComponent& camComp, TransformComponent& camTransform, TransformComponent& targetTransformComp)
{
	camComp.front = glm::normalize(targetTransformComp.position - camTransform.position);
	camComp.right = glm::normalize(glm::cross(camComp.front, camComp.up));
}