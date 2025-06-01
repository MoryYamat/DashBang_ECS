#include "InputRouterSystem.h"

#include <iostream>

void Game::Input::InputRouterSystem(eNsECS::EntityMgr& ecs, const eNsInput::RawInputState& rawInput, const InputMapping& mapping)
{
	for (eNsECS::Entity e : ecs.view<gNsInput::InputActionComponent>())
	{
		auto& inputComp = ecs.get<gNsInput::InputActionComponent>(e);

		inputComp.previous = inputComp.current;
		inputComp.current.clear();

		for (const auto& [key, isDown] : rawInput.keyState)
		{
			if (!isDown) continue;

			if (auto actionOpt = mapping.getAction(key))
			{
				inputComp.current[*actionOpt] = true;
				//std::cout << "[InputRouterSystem.cpp]: " << key << std::endl;
			}
		}
	}
}