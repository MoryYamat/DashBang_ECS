//#include "CameraControlSystem.h"
//
//// Core/ECS
//#include "Engine/ECS/Component/Common/TransformComponent.h"
//#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
//
//// Core/Window
//#include "Engine/Window/WindowManager.h"
//
//#include <iostream>
//#include <algorithm>
//
////
//// カメラ自体が直接Inputを受けて「動いている」
//// 本当は、
//// プレイヤー(PlayerActor)がInputを受け取り
//// カメラ(CameraActor)がPlayerActorに追従する
//// という形がいいと思う
////
//
//
//// 現在未使用 削除予定
//
//// The internal state of ‘InputState’ is not rewritten here.
//// The internal state of ‘InputState’ is not rewritten here.
//// The internal state of ‘InputState’ is not rewritten here.
//// ‘InputState‘の内部状態はここでは書き換えない
//void Game::Camera::Input::UpdateCamera(Engine::ECS::EntityMgr& ecs, Game::Input::InputState& input, float deltaTime)
//{
//	namespace Comp = Engine::ECS::Component;
//	namespace gInput = Game::Input;
//
//	for (Engine::ECS::Entity entity : ecs.view<Comp::Common::TransformComponent, Comp::Camera::CameraComponent, Comp::Tags::PlayerControllerComponent>())
//	{
//		auto& transformComp = ecs.get<Comp::Common::TransformComponent>(entity);
//		auto& cameraComp = ecs.get<Comp::Camera::CameraComponent>(entity);
//
//		float velocity = cameraComp.moveSpeed * deltaTime;
//
//		// It defines the movement
//		// It defines the movement
//		// It defines the movement
//		// It defines the movement
//		if (input.isPressed(gInput::InputAction::MoveForward))
//			transformComp.position += cameraComp.front * velocity;
//		if (input.isPressed(gInput::InputAction::MoveBackward))
//			transformComp.position -= cameraComp.front * velocity;
//		if (input.isPressed(gInput::InputAction::MoveRight))
//			transformComp.position += cameraComp.right * velocity;
//		if (input.isPressed(gInput::InputAction::MoveLeft))
//			transformComp.position -= cameraComp.right * velocity;
//
//
//
//		// It defines the movement
//		// It defines the movement
//		// It defines the movement
//		// It defines the movement
//		if (input.mouseCaptured && input.mouseDelta != glm::vec2(0.0f))
//		{
//			cameraComp.yaw += input.mouseDelta.x * cameraComp.mouseSensitivity;
//
//			// 上下反転
//			cameraComp.pitch -= input.mouseDelta.y * cameraComp.mouseSensitivity;
//
//			// pitch制限
//			cameraComp.pitch = std::clamp(cameraComp.pitch, -89.0f, 89.0f);
//			updateCameraVector(cameraComp);
//		}
//
//		if (input.requestMouseCaptureToggle)
//		{
//			if (input.mouseCaptured)
//			{
//				Engine::Window::WindowManager::CaptureMouse();
//			}
//			else
//			{
//				Engine::Window::WindowManager::ReleaseMouse();
//			}
//
//			// fix later
//			input.requestMouseCaptureToggle = false;
//		}
//
//		//// for debugging
//		//std::cout << "[CameraControlSystem.cpp]: camera Position: x::" << transformComp.position.x
//		//	<< " y::" << transformComp.position.y
//		//	<< " z::" << transformComp.position.z
//		//	<< std::endl;
//		break;
//	}
//}
//
//// 
//void Game::Camera::Input::updateCameraVector(Engine::ECS::Component::Camera::CameraComponent& cameraComp)
//{
//	glm::vec3 front;
//	front.x = cos(glm::radians(cameraComp.yaw)) * cos(glm::radians(cameraComp.pitch));
//	front.y = sin(glm::radians(cameraComp.pitch));
//	front.z = sin(glm::radians(cameraComp.yaw)) * cos(glm::radians(cameraComp.pitch));
//
//	cameraComp.front = glm::normalize(front);
//	cameraComp.right = glm::normalize(glm::cross(cameraComp.front, glm::vec3(0.0f, 1.0f, 0.0f)));
//	cameraComp.up = glm::normalize(glm::cross(cameraComp.right, cameraComp.front));
//}