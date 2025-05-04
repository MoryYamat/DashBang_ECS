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
	

	// �i�J�����̈ʒu�����f���̐��ʃx�N�g���̑Ίp�����ɂ���悤�ɃJ�����̈ʒu�ƌ���������������e���v���[�g�j�i�قڃf�o�b�O�p�j
	void initializeFollowCameraGetFront(FollowCameraComponent& followCamComp, Logic2DTransformComponent& targetLogic2DTransform);

	void initializeCameraVectors(CameraComponent& camComp, TransformComponent& camTransform, TransformComponent& targetTransformComp);

};