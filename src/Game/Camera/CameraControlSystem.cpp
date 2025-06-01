#include "CameraControlSystem.h"

// Core/ECS
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

// Core/Window
#include "Engine/Window/WindowManager.h"

#include <iostream>
#include <algorithm>

//
// ƒJƒƒ‰©‘Ì‚ª’¼ÚInput‚ğó‚¯‚Äu“®‚¢‚Ä‚¢‚év
// –{“–‚ÍA
// ƒvƒŒƒCƒ„[(PlayerActor)‚ªInput‚ğó‚¯æ‚è
// ƒJƒƒ‰(CameraActor)‚ªPlayerActor‚É’Ç]‚·‚é
// ‚Æ‚¢‚¤Œ`‚ª‚¢‚¢‚Æv‚¤
//


// The internal state of eInputStatef is not rewritten here.
// The internal state of eInputStatef is not rewritten here.
// The internal state of eInputStatef is not rewritten here.
// eInputStatee‚Ì“à•”ó‘Ô‚Í‚±‚±‚Å‚Í‘‚«Š·‚¦‚È‚¢
void Game::Camera::Input::UpdateCamera(eNsECS::EntityMgr& ecs, gNsInput::InputState& input, float deltaTime)
{

	for (eNsECS::Entity entity : ecs.view<eNsCommonComp::TransformComponent, eNsCamComp::CameraComponent, eNsTagComp::PlayerControllerComponent>())
	{
		auto& transformComp = ecs.get<eNsCommonComp::TransformComponent>(entity);
		auto& cameraComp = ecs.get<eNsCamComp::CameraComponent>(entity);

		float velocity = cameraComp.moveSpeed * deltaTime;

		// It defines the movement
		// It defines the movement
		// It defines the movement
		// It defines the movement
		if (input.isPressed(gNsInput::InputAction::MoveForward))
			transformComp.position += cameraComp.front * velocity;
		if (input.isPressed(gNsInput::InputAction::MoveBackward))
			transformComp.position -= cameraComp.front * velocity;
		if (input.isPressed(gNsInput::InputAction::MoveRight))
			transformComp.position += cameraComp.right * velocity;
		if (input.isPressed(gNsInput::InputAction::MoveLeft))
			transformComp.position -= cameraComp.right * velocity;



		// It defines the movement
		// It defines the movement
		// It defines the movement
		// It defines the movement
		if (input.mouseCaptured && input.mouseDelta != glm::vec2(0.0f))
		{
			cameraComp.yaw += input.mouseDelta.x * cameraComp.mouseSensitivity;

			// ã‰º”½“]
			cameraComp.pitch -= input.mouseDelta.y * cameraComp.mouseSensitivity;

			// pitch§ŒÀ
			cameraComp.pitch = std::clamp(cameraComp.pitch, -89.0f, 89.0f);
			updateCameraVector(cameraComp);
		}

		if (input.requestMouseCaptureToggle)
		{
			if (input.mouseCaptured)
			{
				eNsWindow::WindowManager::CaptureMouse();
			}
			else
			{
				eNsWindow::WindowManager::ReleaseMouse();
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
void Game::Camera::Input::updateCameraVector(eNsCamComp::CameraComponent& cameraComp)
{
	glm::vec3 front;
	front.x = cos(glm::radians(cameraComp.yaw)) * cos(glm::radians(cameraComp.pitch));
	front.y = sin(glm::radians(cameraComp.pitch));
	front.z = sin(glm::radians(cameraComp.yaw)) * cos(glm::radians(cameraComp.pitch));

	cameraComp.front = glm::normalize(front);
	cameraComp.right = glm::normalize(glm::cross(cameraComp.front, glm::vec3(0.0f, 1.0f, 0.0f)));
	cameraComp.up = glm::normalize(glm::cross(cameraComp.right, cameraComp.front));
}