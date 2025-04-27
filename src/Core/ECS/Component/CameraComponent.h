// camera datas
#pragma once

#include <glm/glm.hpp>

struct CameraComponent
{
	float fov;
	float aspect;
	float nearClip;
	float farClip;

	float yaw;
	float pitch;

	glm::vec3 target;

	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 rigth = glm::vec3(1.0f, 0.0f, 0.0f);

	float moveSpeed = 5.0f;
	float mouseSensitivity = 0.1f;
};