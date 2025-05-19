// System-wide input management system
// �V�X�e���S�̂̃C���v�b�g�Ǘ��V�X�e��

#pragma once

#include <GLM/glm.hpp>
#include <GLFW/glfw3.h>

#include "Core/InputManager/RawInputState.h"


class InputManager
{
public:
	
	InputManager(GLFWwindow* window);
	void Update();// ���t���[��RawInputState���X�V

	// Raw Input getter
	const RawInputState& GetRawInput() { return mRawInput; }
	RawInputState& GetRawInputMutable() { return mRawInput; }// ���������p


	// �����I��MappingContext�Ȃǂ��ێ�����

private:
	// glfw window context
	GLFWwindow* mWindow;

	// raw input struct
	RawInputState mRawInput;

	// first mouse
	bool mFirstMouse;

	// 
	glm::vec2 mLastMousePosition;


	void updateMouseDelta();
	void updateKeyStates();
};