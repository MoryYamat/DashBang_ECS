// RawInput + Mapping → ActionComponent に変換	
// Convert RawInput + Mapping to ActionComponent

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/InputManager/Public/InputFwd.hpp"
#include "Game/Input/InputMapping.h"
#include "Game/Input/InputActionComponent.h"





namespace Game::Input
{
	// GLFWの生の入力をComponentベースの抽象入力に割り当てる
	void InputRouterSystem(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput);

	//	GLFWの生の入力をComponentの抽象入力に割り当てる(ECSグローバルリソース実装後未使用)
	void InputRouterSystem(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput, const Game::Input::InputMapping& mapping);
}