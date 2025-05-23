// Input Component

#pragma once

#include <unordered_map>

//#include "Game/Input/InputState.h"

#include "Core/ECS/Component/InputActionComponent.h"

//enum class InputAction
//{
//	MoveForward,
//	MoveBackward,
//	MoveLeft,
//	MoveRight,
//	Jump,
//	LookAround,
//	MouseCapture,
//	Quit,
//};

struct InputComponent
{
	std::unordered_map<InputAction, bool> actions;
	std::unordered_map<InputAction, bool> previousActions;

	// 現在の状態
	bool isPressed(InputAction action) const 
	{
		auto it = actions.find(action);
		return it != actions.end() && it->second;
	}

	// 1フレームだけ判定
	bool isJustPressed(InputAction action) const
	{
		return isPressed(action) && !wasPressed(action);
	}

	// 今まで押されていたかどうか
	bool wasPressed(InputAction action) const
	{
		auto it = previousActions.find(action);
		return it != actions.end() && it->second;
	}
};