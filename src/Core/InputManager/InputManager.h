// System-wide input management system
// システム全体のインプット管理システム
// 
// 
// singletonにしたい．(後で)
#pragma once

#include <GLM/glm.hpp>
#include <GLFW/glfw3.h>

#include "Core/InputManager/RawInputState.h"


class InputManager
{
public:

	InputManager(GLFWwindow* window);
	void Update();// 毎フレームRawInputStateを更新

	const RawInputState& GetRawInput() { return mRawInput; }
	RawInputState& GetRawInputMutable() { return mRawInput; }// 書き換え用


	// 将来的にMappingContextなども保持する

private:
	GLFWwindow* mWindow;
	RawInputState mRawInput;
	bool mFirstMouse;
	glm::vec2 mLastMousePosition;


	void updateMouseDelta();
	void updateKeyStates();
};