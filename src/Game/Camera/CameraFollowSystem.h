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



	// �s�v�����i�J�����̈ʒu�����f���̐��ʃx�N�g���̑Ίp�����ɂ���悤�ɃJ�����̈ʒu�ƌ���������������e���v���[�g�j
	void setFollowCameraGetFront(FollowCameraComponent& followCamComp, Logic2DTransformComponent& targetLogic2DTransform);
	
	// �������͕K�v����
	void setCameraVectors(CameraComponent& camComp, TransformComponent& camTransform, TransformComponent& targetTransformComp);

};