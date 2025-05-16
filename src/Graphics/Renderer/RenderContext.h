// Render Data

#pragma once

#include "Core/Window/Window.h"

#include <GLM/glm.hpp>

struct RenderContext
{
	glm::vec3 cameraPosition = glm::vec3(1.0f);// �J�����ʒu
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);// �J����front�x�N�g��
	glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);// �J����Right�x�N�g��
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);// �J����Up�x�N�g��
	glm::mat4 viewMatrix = glm::mat4(1.0f);// view�s��
	glm::mat4 projectionMatrix = glm::mat4(1.0f);// ���e�s��
	glm::vec4 viewport = glm::vec4(1.0f);// viewport
};

