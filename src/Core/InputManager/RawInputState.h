// Raw input data available from GLFW
// GLFWから取得できる生の入力データ

#pragma once 

#include <GLM/glm.hpp>

#include <unordered_map>


struct RawInputState
{
	std::unordered_map<int, bool> keyState;
	std::unordered_map<int, bool> previousKeyState;

	// マウスデータ
	glm::vec2 mousePosition = glm::vec2(0.0f);
	glm::vec2 mouseDelta = glm::vec2(0.0f);
	float scrollDelta = 0.0f;


	void ClearTransient() {
		scrollDelta = 0.0f;
		mouseDelta = glm::vec2(0.0f);
	}
};