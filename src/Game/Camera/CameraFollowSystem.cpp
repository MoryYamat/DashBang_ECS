#include "CameraFollowSystem.h"

#include "Engine/ECS/Component/Camera/CameraComponent.h"



#include "Engine/Debug/Private/DebugUtils.h"

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

// 使用中
void Game::Camera::Update(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	namespace Comp = Engine::ECS::Component;

	for (Engine::ECS::Entity e : ecs.view<Comp::Camera::FollowCameraComponent, Comp::Common::TransformComponent, Comp::Camera::CameraComponent>())
	{


		auto& followCam = ecs.get<Comp::Camera::FollowCameraComponent>(e);
		auto& camTransform = ecs.get<Comp::Common::TransformComponent>(e);
		auto& camComp = ecs.get<Comp::Camera::CameraComponent>(e);



		// If the targetEntity does not have a TransformComponent
		if (!ecs.hasComponent<Comp::Common::TransformComponent>(followCam.targetEntity)) return;

		const auto& targetLogic2DTransform = ecs.get<Comp::Logic2D::Logic2DTransformComponent>(followCam.targetEntity);
		Comp::Common::TransformComponent targetTransformComp = ecs.get<Comp::Common::TransformComponent>(followCam.targetEntity);


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

void Game::Camera::setFollowCameraGetFront(Engine::ECS::Component::Camera::FollowCameraComponent& followCamComp
	, Engine::ECS::Component::Logic2D::Logic2DTransformComponent& targetLogic2DTransform
)
{
	// Calculating camera position
	// 論理データのfrontに対して、対角上に存在するように計算している
	float yawDegrees = targetLogic2DTransform.rotation;
	float yawRadians = glm::radians(yawDegrees);


	followCamComp.offset = glm::rotate(followCamComp.offset, yawRadians, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Game::Camera::setCameraVectors(Engine::ECS::Component::Camera::CameraComponent& camComp
	, Engine::ECS::Component::Common::TransformComponent& camTransform
	, Engine::ECS::Component::Common::TransformComponent& targetTransformComp
)
{
	// この計算式
	// frontOffset
	camComp.front = glm::normalize(targetTransformComp.position - camTransform.position + camComp.frontOffset);
	camComp.right = glm::normalize(glm::cross(camComp.front, camComp.up));
}