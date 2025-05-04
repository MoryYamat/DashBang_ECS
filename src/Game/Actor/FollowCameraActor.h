// A camera actor that follows the PlayerCharacter

#pragma once

#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/CameraComponent.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/FollowCameraComponent.h"


class FollowCameraActor
{
public:
	FollowCameraActor(ECS& ecs);


private:
	

	// （カメラの位置がモデルの正面ベクトルの対角方向にくるようにカメラの位置と向きを初期化するテンプレート）（ほぼデバッグ用）
	void initializeFollowCameraGetFront(FollowCameraComponent& followCamComp, Logic2DTransformComponent& targetLogic2DTransform);

	void initializeCameraVectors(CameraComponent& camComp, TransformComponent& camTransform, TransformComponent& targetTransformComp);

};