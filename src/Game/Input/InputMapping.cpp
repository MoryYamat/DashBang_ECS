#include "InputMapping.h"

#include "Core/Window/WindowManager.h"

#include <iostream>


void InputMapping::update(GLFWwindow* window, InputState& input)
{
	input.actions.clear();
	for (const auto& [key, action] : keyBindings)
	{
		bool isPressedNow = glfwGetKey(window, key) == GLFW_PRESS;

		// ����������񂾂���������
		// Process a long press only once
		if (isPressedNow && !input.previousActions[action])
		{
			// �}�E�X�L���v�`�����[�h�̕ύX
			// Change mouse capture mode
			if (action == InputAction::MouseCapture)
			{
				input.mouseCaptured = !input.mouseCaptured;
				input.requestMouseCaptureToggle = true;
				input.firstMouse = true;
			}
		}

		input.actions[action] = isPressedNow;
	}

	// �}�E�X���擾
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec2 currentMouse = glm::vec2(xpos, ypos);
	input.screenMousePosition = currentMouse;

	//std::cout << "[InputMapping.cpp]: mousepos x. " << xpos << " y. " << ypos << std::endl;

	// �}�E�X�̉�ʏ�̕ψʂ��v�Z
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


	// �ꏊ�����������H
	// �ꏊ�����������H
	// �ꏊ�����������H
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