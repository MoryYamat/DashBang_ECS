// update transformcomponent -> stateless
#pragma once

#include "Core/ECS/EntityManager.h"


#include "Core/ECS/Component/CameraComponent.h"


#include "Game/Input/InputState.h"

namespace GameSystemInput
{
	void UpdateCamera(class ECS& ecs, const InputState& input, float deltaTime);

	void updateCameraVector(CameraComponent& cameraComp);
}