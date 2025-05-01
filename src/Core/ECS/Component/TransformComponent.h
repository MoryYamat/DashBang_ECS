// Position, rotation and scale information in world space
// ワールド空間での、位置・回転・スケールの情報
#pragma once

#include "Core/ECS/Component/ModelSizeComponent.h"

#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct TransformComponent
{
	glm::vec3 position = glm::vec3(0.0f);
	// need to convert to a quaternion
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);


	glm::mat4 toMatrix() const {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, scale);
		return model;
	}

	// 実際に表示されている大きさの取得
	// Get the actual size of the image
	glm::vec3 GetWorldSize(const ModelSizeComponent& modelSize) const
	{
		return modelSize.GetSize() * scale;
	}
};

