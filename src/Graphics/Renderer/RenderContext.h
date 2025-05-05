// Render Data

#pragma once

#include <GLM/glm.hpp>

struct RenderContext
{
	glm::mat4 viewMatrix = glm::mat4(1.0f);// view行列
	glm::mat4 projectionMatrix = glm::mat4(1.0f);// 投影行列
	glm::vec3 cameraPosition = glm::vec3(1.0f);// カメラ位置
	glm::vec4 viewport = glm::vec4(1.0f);// viewport
};