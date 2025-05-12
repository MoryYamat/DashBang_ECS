// Special template for initializing FollowCameraComponent
// FollowCameraComponent初期化用の特殊テンプレート

#pragma once

#include "Core/ECS/Meta/InitComponent/InitSystem.h"
#include "Core/EntityUtils/EntityUtils.h"

#include "Core/Window/WindowManager.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/FollowCameraComponent.h"
#include "Core/ECS/Component/CameraComponent.h"

// tags
#include "Core/ECS/Component/Tags/PlayerControllerComponent.h"


#include "Debug/DebugUtils.h"

template<>
struct InitSystem<FollowCameraComponent>
{
	static void Init(FollowCameraComponent& followCamComp, ECS& ecs, Entity cameraEntity)
	{
		Entity ePlayerActor = EntityUtils::getEntityByName(ecs, "Player");
		if (ePlayerActor == Entity::INVALID)
		{
			std::cerr << "[FollowCameraInit.cpp]: Failed to find Player entity by name!" << std::endl;
			return;
		}

		followCamComp.targetEntity = ePlayerActor;
		// Relative coordinates
		followCamComp.offset = { 0.0f, 10.0f, 10.0f };

		// get 
		TransformComponent& targetTransform = ecs.get<TransformComponent>(ePlayerActor);
		Logic2DTransformComponent& targetLogic2DTransform = ecs.get<Logic2DTransformComponent>(ePlayerActor);

		// Initialize camera position
		auto& camTransform = ecs.get<TransformComponent>(cameraEntity);
		camTransform.position = targetTransform.position + followCamComp.offset;

		
		// Initialize camera Front vector
		auto& camComp = ecs.get<CameraComponent>(cameraEntity);
		camComp.aspect = WindowManager::GetAspect();
		camComp.front = glm::normalize(targetTransform.position - camTransform.position);
		camComp.right = glm::normalize(glm::cross(camComp.front, camComp.up));



		DebugUtils::LogVector("FollowCameraActor.cpp", camComp.front);

	}
};