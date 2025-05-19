#include "InputRouterSystem.h"

#include <iostream>

void InputRouterSystem(ECS& ecs, const RawInputState& rawInput, const InputMapping& mapping)
{
	for (Entity e : ecs.view<InputActionComponent>())
	{
		auto& inputComp = ecs.get<InputActionComponent>(e);

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