
#include "CameraActor.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Camera/CameraComponent.h"
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Engine/Graphics/Renderer/Shader.h"

#include "Engine/Debug/DebugUtils.h"

Game::Actor::Camera::CameraActor::CameraActor(eNsECS::EntityMgr& ecs)
{
	eNsECS::Entity entity = ecs.createEntity();

	eNsCommonComp::TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.rotation = glm::vec3(0.0f);
	transformComp.scale = glm::vec3(1.0f);
	ecs.addComponent(entity, transformComp);

	eNsCamComp::CameraComponent cameraComp;
	cameraComp.fov = 60.0f;
	cameraComp.target = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraComp.up = glm::vec3(0.0f, 1.0f, 0.0f);

	eNsTagComp::PlayerControllerComponent player;
	ecs.addComponent(entity, player);

	cameraComp.aspect = 1280.0f / 720.0f;
	cameraComp.nearClip = 0.1f;
	cameraComp.farClip = 100.0f;

	ecs.addComponent(entity, cameraComp);


	//// For Debugging
	//DebugUtils::printEntityComponents<
	//	TransformComponent,
	//	CameraComponent,
	//	PlayerControllerComponent
	//>(ecs, entity);
}