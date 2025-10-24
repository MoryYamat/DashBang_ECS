#include "InputRouterSystem.h"

#include "Game/Input/Private/InputMapping.h"
#include "Game/Input/Private/InputActionComponent.h"

#include "Engine/InputManager/Private/RawinputState.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <iostream>

void Game::Input::InputRouterSystem(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput)
{
	auto& map = ecs.getResource<Game::Input::InputMapping>();

	for (Engine::ECS::Entity e : ecs.view<Game::Input::InputActionComponent>())
	{
		auto& inputComp = ecs.get<Game::Input::InputActionComponent>(e);

		inputComp.previous = inputComp.current;
		inputComp.current.clear();

		for (const auto& [key, isDown] : rawInput.keyState)
		{
			if (!isDown) continue;

			if (auto actionOpt = map.getAction(key))
			{
				inputComp.current[*actionOpt] = true;
			}
		}
	}
}

namespace Game::Input
{
	void InputRouterSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}
}