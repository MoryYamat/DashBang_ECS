#include "Game/Actor/FollowCameraActor.h"

#include "Engine/Window/WindowManager.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Engine/ECS/EntityUtils/EntityUtils.h"

#include "Engine/Debug/Private/DebugUtils.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <glm/gtx/rotate_vector.hpp>


#include <cstdint>
#include <iostream>

Game::Actor::Camera::FollowCameraActor::FollowCameraActor(Engine::ECS::EntityMgr& ecs)
{
	Engine::ECS::Entity entity = ecs.createEntity();

	//CameraComponent camComp;
	//FollowCameraComponent followCamComp;
	//TransformComponent CamTransformComp;

	Engine::ECS::Ops::Add<Engine::ECS::Component::Camera::CameraComponent>(ecs, entity, Engine::ECS::Component::Camera::CameraComponent{});
	Engine::ECS::Ops::Add<Engine::ECS::Component::Camera::FollowCameraComponent>(ecs, entity, Engine::ECS::Component::Camera::FollowCameraComponent{});
	Engine::ECS::Ops::Add<Engine::ECS::Component::Common::TransformComponent>(ecs, entity, Engine::ECS::Component::Common::TransformComponent{});

	//camComp.aspect = WindowManager::GetAspect();
	// initialize camera vectors

	// Get the ID of the target entity to follow and set it to target
	//Entity ePlayerActor = EntityUtils::getEntityByName(ecs, "Player");
	//if (ePlayerActor != Entity::INVALID)
	//{
	//	followCamComp.targetEntity = ePlayerActor;
	//	ecs.addComponent(entity, followCamComp);
	//	TransformComponent& targetTransform = ecs.get<TransformComponent>(ePlayerActor);

	//	// Relative coordinates
	//	followCamComp.offset = { 0.0f, 10.0f, 10.0f };
	//	
	//	Logic2DTransformComponent& targetLogic2DTransform = ecs.get<Logic2DTransformComponent>(ePlayerActor);
	//	

	//	initializeFollowCameraGetFront(followCamComp, targetLogic2DTransform);
	//	// Initialize camera position
	//	CamTransformComp.position = targetTransform.position + followCamComp.offset;

	//	initializeCameraVectors(camComp, CamTransformComp, targetTransform);
	//	
	//	ecs.addComponent(entity, CamTransformComp);
	//	ecs.addComponent(entity, camComp);

	//	
	//}
	//else
	//{
	//	std::cerr << "[FollowCameraActor.cpp]: Failed to find Player entity by name!" << std::endl;
	//}

	//DebugUtils::LogVector("FollowCameraActor.cpp", camComp.front);
	//DebugUtils::LogVector("FollowCameraActor.cpp", CamTransformComp.position);
}


// 現在未使用
void Game::Actor::Camera::FollowCameraActor::initializeFollowCameraGetFront(
	Engine::ECS::Component::Camera::FollowCameraComponent& followCamComp
		, Engine::ECS::Component::Logic2D::Logic2DTransformComponent& targetLogic2DTransform
	)
{
	// Calculating camera position
	// 論理データのfrontに対して、対角上に存在するように計算している
	float yawDegrees = targetLogic2DTransform.rotation;
	float yawRadians = glm::radians(yawDegrees);


	followCamComp.offset = glm::rotate(followCamComp.offset, yawRadians, glm::vec3(0.0f, 1.0f, 0.0f));
}


// 現在未使用
// Calculates the coordinate data for drawing the camera component from logical data
void Game::Actor::Camera::FollowCameraActor::initializeCameraVectors(
	Engine::ECS::Component::Camera::CameraComponent& camComp
	, Engine::ECS::Component::Common::TransformComponent& camTransform
	, Engine::ECS::Component::Common::TransformComponent& targetTransformComp)
{
	// debugging
	Engine::Debug::Logging::GeneralLog("FollowCameraActor.cpp", "Initializing camera vectors...");

	camComp.front = glm::normalize(targetTransformComp.position - camTransform.position);
	camComp.right = glm::normalize(glm::cross(camComp.front, camComp.up));
	//camComp.pitch = glm::degrees(glm::asin(camComp.front.y));
	//camComp.yaw = glm::degrees(glm::atan(camComp.front.x, camComp.front.z));
}
