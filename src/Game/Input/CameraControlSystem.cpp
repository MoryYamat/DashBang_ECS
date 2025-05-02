#include "CameraControlSystem.h"

// Core/ECS
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/PlayerControllerComponent.h"

// Core/Window
#include "Core/Window/WindowManager.h"

#include <iostream>
#include <algorithm>

//
// �J�������̂�����Input���󂯂āu�����Ă���v
// �{���́A
// �v���C���[(PlayerActor)��Input���󂯎��
// �J����(CameraActor)��PlayerActor�ɒǏ]����
// �Ƃ����`�������Ǝv��
//


// The internal state of �eInputState�f is not rewritten here.
// The internal state of �eInputState�f is not rewritten here.
// The internal state of �eInputState�f is not rewritten here.
// �eInputState�e�̓�����Ԃ͂����ł͏��������Ȃ�
void GameSystemInput::UpdateCamera(ECS& ecs, InputState& input, float deltaTime)
{

	for (auto entity : ecs.view<TransformComponent, CameraComponent, PlayerControllerComponent>())
	{
		auto& transformComp = ecs.get<TransformComponent>(entity);
		auto& cameraComp = ecs.get<CameraComponent>(entity);

		float velocity = cameraComp.moveSpeed * deltaTime;

		// It defines the movement
		// It defines the movement
		// It defines the movement
		// It defines the movement
		if (input.isPressed(InputAction::MoveForward))
			transformComp.position += cameraComp.front * velocity;
		if (input.isPressed(InputAction::MoveBackward))
			transformComp.position -= cameraComp.front * velocity;
		if (input.isPressed(InputAction::MoveRight))
			transformComp.position += cameraComp.right * velocity;
		if (input.isPressed(InputAction::MoveLeft))
			transformComp.position -= cameraComp.right * velocity;



		// It defines the movement
		// It defines the movement
		// It defines the movement
		// It defines the movement
		if (input.mouseCaptured && input.mouseDelta != glm::vec2(0.0f))
		{
			cameraComp.yaw += input.mouseDelta.x * cameraComp.mouseSensitivity;

			// �㉺���]
			cameraComp.pitch -= input.mouseDelta.y * cameraComp.mouseSensitivity;

			// pitch����
			cameraComp.pitch = std::clamp(cameraComp.pitch, -89.0f, 89.0f);
			GameSystemInput::updateCameraVector(cameraComp);
		}

		if (input.requestMouseCaptureToggle)
		{
			if (input.mouseCaptured)
			{
				WindowManager::CaptureMouse();
			}
			else
			{
				WindowManager::ReleaseMouse();
			}

			// fix later
			input.requestMouseCaptureToggle = false;
		}

		//// for debugging
		//std::cout << "[CameraControlSystem.cpp]: camera Position: x::" << transformComp.position.x
		//	<< " y::" << transformComp.position.y
		//	<< " z::" << transformComp.position.z
		//	<< std::endl;
		break;
	}
}

// 
void GameSystemInput::updateCameraVector(CameraComponent& cameraComp)
{
	glm::vec3 front;
	front.x = cos(glm::radians(cameraComp.yaw)) * cos(glm::radians(cameraComp.pitch));
	front.y = sin(glm::radians(cameraComp.pitch));
	front.z = sin(glm::radians(cameraComp.yaw)) * cos(glm::radians(cameraComp.pitch));

	cameraComp.front = glm::normalize(front);
	cameraComp.right = glm::normalize(glm::cross(cameraComp.front, glm::vec3(0.0f, 1.0f, 0.0f)));
	cameraComp.up = glm::normalize(glm::cross(cameraComp.right, cameraComp.front));
}