// Render Data

#pragma once

#include <GLM/glm.hpp>

struct RenderContext
{
	glm::mat4 viewMatrix = glm::mat4(1.0f);// view�s��
	glm::mat4 projectionMatrix = glm::mat4(1.0f);// ���e�s��
	glm::vec3 cameraPosition = glm::vec3(1.0f);// �J�����ʒu
	glm::vec4 viewport = glm::vec4(1.0f);// viewport
};