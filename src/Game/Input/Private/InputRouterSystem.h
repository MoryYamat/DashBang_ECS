// RawInput + Mapping → ActionComponent に変換	
// Convert RawInput + Mapping to ActionComponent

#pragma once

#include "Engine/ECS/EntityManager.h"

//
#include "Engine/InputManager/Public/InputFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"



namespace Game::Input
{
	// GLFWの生の入力をComponentベースの抽象入力に割り当てる
	void InputRouterSystem(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput);

	void InputRouterSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
}