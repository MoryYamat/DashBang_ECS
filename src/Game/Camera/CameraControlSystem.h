// update transformcomponent -> stateless
#pragma once

#include "Engine/ECS/EntityManager.h"


#include "Engine/ECS/Component/Camera/CameraComponent.h"


#include "Game/Input/InputState.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Camera::Input
{
	void UpdateCamera(eNsECS::EntityMgr& ecs, gNsInput::InputState& input, float deltaTime);

	void updateCameraVector(eNsCamComp::CameraComponent& cameraComp);
}