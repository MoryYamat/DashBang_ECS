// Special template for initializing FollowCameraComponent
// FollowCameraComponent初期化用の特殊テンプレート

#pragma once

#include "Engine/ECS/Meta/InitComponent/InitSystem.h"
#include "Engine/ECS/EntityUtils/EntityUtils.h"

// #include "Core/Window/WindowManager.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"
#include "Engine/ECS/Component/Camera/CameraComponent.h"

// tags
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"


#include "Engine/Debug/DebugUtils.h"

#include "Common/EngineNamespaceDecl.h"

#include "Common/GameNamespaceDecl.h"

#include <iostream>

// 実質的なカメラ初期化用システム
// TODO: カメラの微調整のために完成間近に詳細を詰める必要あり
namespace Engine::ECS::Meta::Init
{
	// 名前空間をゲーム側にする必要あり
	template<>
	struct InitSystem<eNsCamComp::FollowCameraComponent>
	{
		static void Init(eNsCamComp::FollowCameraComponent& followCamComp, eNsECS::EntityMgr& ecs, Entity cameraEntity, eNsWindow::Window& window)
		{
			Entity ePlayerActor = EntityUtils::getEntityByName(ecs, "Player");
			if (ePlayerActor == Entity::INVALID)
			{
				std::cerr << "[FollowCameraInit.cpp]: Failed to find Player entity by name!" << std::endl;
				return;
			}

			followCamComp.targetEntity = ePlayerActor;
			// Relative coordinates
			followCamComp.offset = { 0.0f, 7.0f,5.0f };
			//followCamComp.offset = { 0.0f, 30.0f,20.0f };

			// get 
			eNsCommonComp::TransformComponent& targetTransform = ecs.get<eNsCommonComp::TransformComponent>(ePlayerActor);
			eNsLogic2DComp::Logic2DTransformComponent& targetLogic2DTransform = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(ePlayerActor);

			// Initialize camera position
			auto& camTransform = ecs.get<eNsCommonComp::TransformComponent>(cameraEntity);
			camTransform.position = targetTransform.position + followCamComp.offset;

			// カギは frontOffset と バネ

			// Initialize camera Front vector
			auto& camComp = ecs.get<eNsCamComp::CameraComponent>(cameraEntity);
			camComp.fov = glm::radians(45.0f);
			camComp.aspect = window.GetAspect();
			camComp.frontOffset = glm::vec3(0.0f, 0.0f, -0.6f);
			camComp.front = glm::normalize(targetTransform.position - camTransform.position + camComp.frontOffset);
			camComp.right = glm::normalize(glm::cross(camComp.front, camComp.up));



			eNsDebugLog::LogVector("FollowCameraInit.h", camComp.front);

			eNsDebugLog::GeneralLog("FollowCameraInit.h", "FollowCamera");
		}
	};
}

