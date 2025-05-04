// Render Data

#pragma once

#include <GLM/glm.hpp>

struct RenderContext
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 cameraPosition;
	glm::vec2 viewportSize;
};