#include "InputRouterSystem.h"

#include "Game/Input/InputMapping.h"

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

void Game::Input::InputRouterSystem(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput, const InputMapping& mapping)
{
	for (Engine::ECS::Entity e : ecs.view<Game::Input::InputActionComponent>())
	{
		auto& inputComp = ecs.get<Game::Input::InputActionComponent>(e);

		inputComp.previous = inputComp.current;
		inputComp.current.clear();

		// 
		for (const auto& [key, isDown] : rawInput.keyState)
		{
			// 押下されていないならばスキップ
			if (!isDown) continue;

			// 押下されている"かつ"マッピングされている場合
			if (auto actionOpt = mapping.getAction(key))
			{
				inputComp.current[*actionOpt] = true;
				//std::cout << "[InputRouterSystem.cpp]: " << key << std::endl;
			}
		}
	}
}