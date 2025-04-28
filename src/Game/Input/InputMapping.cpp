#include "InputMapping.h"

#include <iostream>


void InputMapping::update(GLFWwindow* window, InputState& input)
{
	input.actions.clear();
	for (const auto& [key, action] : keyBindings)
	{
		if (glfwGetKey(window, key) == GLFW_PRESS)
		{
			input.actions[action] = true;
		}
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

}