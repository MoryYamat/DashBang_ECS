#include "InputManager.h"

#include "Debug/DebugUtils.h"

InputManager::InputManager(GLFWwindow* window)
	: mWindow(window)
	, mFirstMouse(true)
	, mLastMousePosition(0.0f)
{
	// 初期マウス位置
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	mLastMousePosition = glm::vec2(x, y);
	mFirstMouse = false;

	DebugUtils::GeneralLog("InputManager.cpp(Initialize)", "InputManager initialization completed successfully");
}

void InputManager::Update()
{
	mRawInput.previousKeyState = mRawInput.keyState;
	mRawInput.mouseDelta = glm::vec2(0.0f);
	mRawInput.scrollDelta = 0.0f;

	updateKeyStates();
	updateMouseDelta();
}

void InputManager::updateKeyStates()
{
	mRawInput.keyState.clear();

	for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
	{
		// state = GLFW_PRESS or GLFW_RELEASE
		int state = glfwGetKey(mWindow, key);
		mRawInput.keyState[key] = (state == GLFW_PRESS || state == GLFW_REPEAT);
	}
}

void InputManager::updateMouseDelta()
{
	double xpos, ypos;
	glfwGetCursorPos(mWindow, &xpos, &ypos);
	glm::vec2 currentMousePos = glm::vec2(xpos, ypos);

	if (mFirstMouse)
	{
		mLastMousePosition = currentMousePos;
		mFirstMouse = false;
	}

	mRawInput.mouseDelta = currentMousePos - mLastMousePosition;
	mRawInput.mousePosition = currentMousePos;

	mLastMousePosition = currentMousePos;
}