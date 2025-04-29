// input mapping

#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <map>

#include "Game/Input/InputState.h"

class InputMapping
{
public:
	InputMapping()
	{
		// This structure cannot withstand adding the ability to change key bindings.
		// This structure cannot withstand adding the ability to change key bindings.
		// This structure cannot withstand adding the ability to change key bindings.
		// This structure cannot withstand adding the ability to change key bindings.
		keyBindings[GLFW_KEY_W] = InputAction::MoveForward;
		keyBindings[GLFW_KEY_S] = InputAction::MoveBackward;
		keyBindings[GLFW_KEY_A] = InputAction::MoveLeft;
		keyBindings[GLFW_KEY_D] = InputAction::MoveRight;
		keyBindings[GLFW_KEY_LEFT_CONTROL] = InputAction::MouseCapture;
		keyBindings[GLFW_KEY_ESCAPE] = InputAction::Quit;
	}

	void update(GLFWwindow* window, InputState& input);

private:
	// mapping
	std::unordered_map<int, InputAction> keyBindings;\
};