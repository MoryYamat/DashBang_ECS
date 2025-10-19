// A camera actor that follows the PlayerCharacter

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Camera/CameraComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"


namespace Game::Actor::Camera
{
	class FollowCameraActor
	{
	public:
		FollowCameraActor(Engine::ECS::EntityMgr& ecs);


	private:

		// 現在未使用
		// （カメラの位置がモデルの正面ベクトルの対角方向にくるようにカメラの位置と向きを初期化するテンプレート）（ほぼデバッグ用）
		void initializeFollowCameraGetFront(Engine::ECS::Component::Camera::FollowCameraComponent& followCamComp
			, Engine::ECS::Component::Logic2D::Logic2DTransformComponent& targetLogic2DTransform);

		void initializeCameraVectors(Engine::ECS::Component::Camera::CameraComponent& camComp
			, Engine::ECS::Component::Common::TransformComponent& camTransform
			, Engine::ECS::Component::Common::TransformComponent& targetTransformComp);

	};
}

