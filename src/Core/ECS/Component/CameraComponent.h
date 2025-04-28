// camera datas
#pragma once

#include <glm/glm.hpp>

struct CameraComponent
{
	float fov = 60.0f;
	float aspect;
	float nearClip = 0.1f;
	float farClip = 100.0f;

	float yaw = -90.0f;
	float pitch = 0.0f;

	glm::vec3 target = glm::vec3(0.0f);

	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

	float moveSpeed = 5.0f;
	float mouseSensitivity = 0.1f;
};