// A camera actor that follows the PlayerCharacter

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Camera/CameraComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Actor::Camera
{
	class FollowCameraActor
	{
	public:
		FollowCameraActor(eNsECS::EntityMgr& ecs);


	private:


		// （カメラの位置がモデルの正面ベクトルの対角方向にくるようにカメラの位置と向きを初期化するテンプレート）（ほぼデバッグ用）
		void initializeFollowCameraGetFront(eNsCamComp::FollowCameraComponent& followCamComp
			, eNsLogic2DComp::Logic2DTransformComponent& targetLogic2DTransform);

		void initializeCameraVectors(eNsCamComp::CameraComponent& camComp
			, eNsCommonComp::TransformComponent& camTransform
			, eNsCommonComp::TransformComponent& targetTransformComp);

	};
}

