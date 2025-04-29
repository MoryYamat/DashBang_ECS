#include "InputMapping.h"

#include "Core/Window/WindowManager.h"

#include <iostream>


void InputMapping::update(GLFWwindow* window, InputState& input)
{
	input.actions.clear();
	for (const auto& [key, action] : keyBindings)
	{
		bool isPressedNow = glfwGetKey(window, key) == GLFW_PRESS;

		if (isPressedNow && !input.previousActions[action])
		{
			if (action == InputAction::MouseCapture)
			{
				input.mouseCaptured = !input.mouseCaptured;
				input.requestMouseCaptureToggle = true;
				input.firstMouse = true;
			}
		}

		input.actions[action] = isPressedNow;
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec2 currentMouse = glm::vec2(xpos, ypos);

	if (input.firstMouse)
	{
		input.mousePosition = currentMouse;
		input.mouseDelta = glm::vec2(0.0f);
		input.firstMouse = false;
	}
	else
	{
		input.mouseDelta = currentMouse - input.mousePosition;
		input.mousePosition = currentMouse;
	}

	// êŠ‚ª‚¨‚©‚µ‚¢H
	// êŠ‚ª‚¨‚©‚µ‚¢H
	// êŠ‚ª‚¨‚©‚µ‚¢H
	//Is the location wrong ?
	//Is the location wrong ?
	//Is the location wrong ?
	//if (input.isPressed(InputAction::MouseCapture))
	//{
	//	input.mouseCaptured = !input.mouseCaptured;
	//	if (input.mouseCaptured)
	//		WindowManager::CaptureMouse();
	//	else
	//		WindowManager::ReleaseMouse();
	//}

	input.previousActions = input.actions;
}