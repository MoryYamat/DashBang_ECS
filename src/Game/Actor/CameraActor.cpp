
#include "CameraActor.h"

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/CameraComponent.h"
#include "Core/ECS/Component/PlayerControllerComponent.h"

#include "Graphics/Renderer/Shader.h"

#include "Debug/DebugUtils.h"

CameraActor::CameraActor(ECS& ecs)
{
	Entity entity = ecs.createEntity();

	TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, -5.0f);
	transformComp.rotation = glm::vec3(0.0f);
	transformComp.scale = glm::vec3(1.0f);

	CameraComponent cameraComp;
	cameraComp.fov = 60.0f;
	cameraComp.target = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraComp.up = glm::vec3(0.0f, 1.0f, 0.0f);

	PlayerControllerComponent player;
	ecs.addComponent(entity, player);

	cameraComp.aspect = 1280.0f / 720.0f;
	cameraComp.nearClip = 0.1f;
	cameraComp.farClip = 100.0f;

	ecs.addComponent(entity, transformComp);
	ecs.addComponent(entity, cameraComp);


	//// For Debugging
	//DebugUtils::printEntityComponents<
	//	TransformComponent,
	//	CameraComponent,
	//	PlayerControllerComponent
	//>(ecs, entity);
}