// Render Data

#pragma once

#include <GLM/glm.hpp>

struct RenderContext
{
	glm::mat4 viewMatrix;// view行列
	glm::mat4 projectionMatrix;// 投影行列
	glm::vec3 cameraPosition;// カメラ位置
	glm::vec4 viewport;// viewport
};