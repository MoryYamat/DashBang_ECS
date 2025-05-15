// Input Component

#pragma once

#include <unordered_map>

//#include "Game/Input/InputState.h"

enum class InputAction
{
	MoveForward,
	MoveBackward,
	MoveLeft,
	MoveRight,
	Jump,
	LookAround,
	MouseCapture,
	Quit,
};

struct InputComponent
{
	std::unordered_map<InputAction, bool> actions;
	std::unordered_map<InputAction, bool> previousActions;

	// Œ»Ý‚Ìó‘Ô
	bool isPressed(InputAction action) const 
	{
		auto it = actions.find(action);
		return it != actions.end() && it->second;
	}

	// 1ƒtƒŒ[ƒ€‚¾‚¯”»’è
	bool isJustPressed(InputAction action) const
	{
		return isPressed(action) && !wasPressed(action);
	}

	// ¡‚Ü‚Å‰Ÿ‚³‚ê‚Ä‚¢‚½‚©‚Ç‚¤‚©
	bool wasPressed(InputAction action) const
	{
		auto it = previousActions.find(action);
		return it != actions.end() && it->second;
	}
};