// RawInput + Mapping ¨ ActionComponent ‚É•ÏŠ·	
// Convert RawInput + Mapping to ActionComponent

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/InputManager/RawInputState.h"
#include "Game/Input/InputMapping.h"
#include "Game/Input/InputActionComponent.h"

#include "Common/EngineNamespaceDecl.h"

#include "Common/GameNamespaceDecl.h"

namespace Game::Input
{
	//	GLFW‚Ì¶‚Ì“ü—Í‚ğComponent‚Ì’ŠÛ“ü—Í‚ÉŠ„‚è“–‚Ä‚é
	void InputRouterSystem(eNsECS::EntityMgr& ecs, const eNsInput::RawInputState& rawInput, const gNsInput::InputMapping& mapping);
}