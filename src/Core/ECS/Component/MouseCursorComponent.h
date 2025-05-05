// Adding mouse meaning to entities
// Storing mouse data
#pragma once

#include <GLM/glm.hpp>

struct MouseCursorComponent
{
	//�@�K�v���v����
	//glm::vec3 worldPosition = { 0.0f, 0.0f, 0.0f };
	bool isVisible = true;
	bool isHoveringUI = false;
	float debugDrawSize = 0.02f;
};