#include "CameraFollowSystem.h"

#include "Engine/ECS/Component/Camera/CameraComponent.h"



#include "Engine/Debug/DebugUtils.h"

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

void Game::Camera::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	for (eNsECS::Entity e : ecs.view<eNsCamComp::FollowCameraComponent, eNsCommonComp::TransformComponent, eNsCamComp::CameraComponent>())
	{


		auto& followCam = ecs.get<eNsCamComp::FollowCameraComponent>(e);
		auto& camTransform = ecs.get<eNsCommonComp::TransformComponent>(e);
		auto& camComp = ecs.get<eNsCamComp::CameraComponent>(e);



		// If the targetEntity does not have a TransformComponent
		if (!ecs.hasComponent<eNsCommonComp::TransformComponent>(followCam.targetEntity)) return;

		const auto& targetLogic2DTransform = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(followCam.targetEntity);
		eNsCommonComp::TransformComponent targetTransformComp = ecs.get<eNsCommonComp::TransformComponent>(followCam.targetEntity);


		camTransform.position = targetTransformComp.position + followCam.offset;

		// 
		setCameraVectors(camComp, camTransform, targetTransformComp);


		//camTransform.position = targetTransform.position + rotatedOffset;
		// 追従(オフセット)
		//camTransform.position = targetTransform.position + followCam.offset;

		// 方向を追従対象へ
		//glm::vec3 direction = glm::normalize(targetTransform.position - camTransform.position);


		// position log for debbugging
		//DebugUtils::LogVector("CameraFollowSystem.cpp", camTransform.position);
		// DebugUtils::LogVector("CameraFollowSystem", camComp.front);

		break;
	}
}

void Game::Camera::setFollowCameraGetFront(eNsCamComp::FollowCameraComponent& followCamComp
	, eNsLogic2DComp::Logic2DTransformComponent& targetLogic2DTransform
)
{
	// Calculating camera position
	// 論理データのfrontに対して、対角上に存在するように計算している
	float yawDegrees = targetLogic2DTransform.rotation;
	float yawRadians = glm::radians(yawDegrees);


	followCamComp.offset = glm::rotate(followCamComp.offset, yawRadians, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Game::Camera::setCameraVectors(eNsCamComp::CameraComponent& camComp
	, eNsCommonComp::TransformComponent& camTransform
	, eNsCommonComp::TransformComponent& targetTransformComp
)
{
	camComp.front = glm::normalize(targetTransformComp.position - camTransform.position);
	camComp.right = glm::normalize(glm::cross(camComp.front, camComp.up));
}