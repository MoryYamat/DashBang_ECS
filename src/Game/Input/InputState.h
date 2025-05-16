// input states

#pragma once

#include <glm/glm.hpp>

#include "Game/Input/InputAction.h"

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

struct InputState
{
	std::unordered_map<InputAction, bool> actions;

	std::unordered_map<InputAction, bool> previousActions;

	// スクリーン上のマウス座標
	glm::vec2 screenMousePosition = glm::vec2(0.0f);
	glm::vec2 viewportSize = glm::vec2(0.0f, 0.0f);

	// マウス変位のためのデータ
	glm::vec2 mousePosition = glm::vec2(0.0f);
	glm::vec2 mouseDelta = glm::vec2(0.0f);

	float scrollDelta = 0.0f;
	bool mouseCaptured = true;
	bool requestMouseCaptureToggle = false;

	bool isPressed(InputAction action) const {
		auto it = actions.find(action);
		return it != actions.end() && it->second;
	}


	bool firstMouse = true;
};