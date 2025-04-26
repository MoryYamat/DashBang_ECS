// transform dataset
#pragma once

#include <GLM/glm.hpp>

struct TransformComponent
{
	glm::vec3 position = glm::vec3(0.0f);
	// need to convert to a quaternion
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

};

