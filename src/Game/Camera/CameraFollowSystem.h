// Camera Follow system

#pragma once
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Camera/CameraComponent.h"
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"

#include "Common/EngineNamespaceDecl.h"

// Camera Following System
namespace Game::Camera
{
	void Update(eNsECS::EntityMgr& ecs, float deltaTime);



	// 不要かも（カメラの位置がモデルの正面ベクトルの対角方向にくるようにカメラの位置と向きを初期化するテンプレート）
	void setFollowCameraGetFront(
		eNsCamComp::FollowCameraComponent& followCamComp
		, eNsLogic2DComp::Logic2DTransformComponent& targetLogic2DTransform
	);
	
	// こっちは必要かも
	void setCameraVectors(
		eNsCamComp::CameraComponent& camComp
		, eNsCommonComp::TransformComponent& camTransform
		, eNsCommonComp::TransformComponent& targetTransformComp
	);

};