
#include "CameraActor.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Camera/CameraComponent.h"
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Engine/Graphics/Private/Renderer/Shader.h"

#include "Engine/Debug/Private/DebugUtils.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

// 現在未使用: 削除予定
Game::Actor::Camera::CameraActor::CameraActor(Engine::ECS::EntityMgr& ecs)
{
	//Engine::ECS::Entity entity = ecs.createEntity();

	//Engine::ECS::Component::Common::TransformComponent transformComp;
	//transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
	//transformComp.rotation = glm::vec3(0.0f);
	//transformComp.scale = glm::vec3(1.0f);
	//Engine::ECS::Ops::Add<Engine::ECS::Component::Common::TransformComponent>(ecs, entity, transformComp);

	//Engine::ECS::Component::Camera::CameraComponent cameraComp;
	//cameraComp.fov = glm::radians(120.0f);
	//cameraComp.target = glm::vec3(0.0f, 0.0f, 0.0f);
	//cameraComp.up = glm::vec3(0.0f, 1.0f, 0.0f);

	//Engine::ECS::Component::Tags::PlayerControllerComponent player;
	//Engine::ECS::Ops::Add<Engine::ECS::Component::Tags::PlayerControllerComponent>(ecs, entity, player);

	//cameraComp.aspect = 1280.0f / 720.0f;
	//cameraComp.nearClip = 0.1f;
	//cameraComp.farClip = 100.0f;

	//Engine::ECS::Ops::Add<Engine::ECS::Component::Camera::CameraComponent>(ecs, entity, cameraComp);


	////// For Debugging
	////DebugUtils::printEntityComponents<
	////	TransformComponent,
	////	CameraComponent,
	////	PlayerControllerComponent
	////>(ecs, entity);
}