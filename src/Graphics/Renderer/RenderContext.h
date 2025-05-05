// Render Data

#pragma once

#include <GLM/glm.hpp>

struct RenderContext
{
	glm::mat4 viewMatrix;// view�s��
	glm::mat4 projectionMatrix;// ���e�s��
	glm::vec3 cameraPosition;// �J�����ʒu
	glm::vec4 viewport;// viewport
};