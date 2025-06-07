// RawInput + Mapping → ActionComponent に変換	
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
	// GLFWの生の入力をComponentベースの抽象入力に割り当てる
	void InputRouterSystem(eNsECS::EntityMgr& ecs, const eNsInput::RawInputState& rawInput);

	//	GLFWの生の入力をComponentの抽象入力に割り当てる(ECSグローバルリソース実装後未使用)
	void InputRouterSystem(eNsECS::EntityMgr& ecs, const eNsInput::RawInputState& rawInput, const gNsInput::InputMapping& mapping);
}