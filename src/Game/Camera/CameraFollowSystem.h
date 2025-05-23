// Camera Follow system

#pragma once
#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/CameraComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/FollowCameraComponent.h"

// Camera Following System
namespace CameraFollowSystem
{
	void Update(ECS& ecs, float deltaTime);



	// 不要かも（カメラの位置がモデルの正面ベクトルの対角方向にくるようにカメラの位置と向きを初期化するテンプレート）
	void setFollowCameraGetFront(FollowCameraComponent& followCamComp, Logic2DTransformComponent& targetLogic2DTransform);
	
	// こっちは必要かも
	void setCameraVectors(CameraComponent& camComp, TransformComponent& camTransform, TransformComponent& targetTransformComp);

};