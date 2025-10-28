#pragma once

#include "InputAction.hpp"

#include <unordered_map>

namespace Game::Character::Input
{
	// 抽象化入力ボタンの(押下)状態
	struct InputActionComponent
	{
		std::unordered_map<InputAction, bool> current;
		std::unordered_map<InputAction, bool> previous;

		bool isPressed(InputAction action)
		{
			auto it = current.find(action);
			return it != current.end() && it->second;
		}

		bool wasPressed(InputAction action)
		{
			auto it = previous.find(action);
			return it != previous.end() && it->second;
		}

		bool isJustPressed(InputAction action)
		{
			return isPressed(action) && !wasPressed(action);
		}

		bool isJustReleased(InputAction action)
		{
			return !isPressed(action) && wasPressed(action);
		}
	};
}