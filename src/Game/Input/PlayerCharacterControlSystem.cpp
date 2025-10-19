#include "PlayerCharacterControlSystem.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Engine/InputManager/InputManager.h"

// collision
#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"
#include "Engine/ECS/Component/Logic2D/ColliderType.h"

#include "Game/Input/InputActionComponent.h"

#include "Engine/Debug/DebugUtils.h"

#include "Game/Utils/SpatialTransformUtils.h"

#include <glm/ext/matrix_projection.hpp>

#include "Engine/Config/CanonicalDefaults.h"

// ========Semantic Layer========
// Character State
// #include "Game/Character/State/Component/CharacterStateComponent.h"
#include "Game/Character/State/System/CharacterStateSystem.h"

#include <cmath>
#include <iostream>


// raw Input と Game Input の分離後 (semantic Layer導入後 廃止)
void Game::Input::Player::Update(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput, Engine::Graphics::Render::RenderContext& renderContext, float deltaTime)
{
	Game::Input::InputActionComponent input;
	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Tags::PlayerControllerComponent, Game::Input::InputActionComponent>())
	{
		input = ecs.get<Game::Input::InputActionComponent>(e);
		break;
	}

	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Tags::PlayerControllerComponent, Engine::ECS::Component::Logic2D::Logic2DTransformComponent, Engine::ECS::Component::Logic2D::CollisionComponent>())
	{
		auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e);
		auto& collisionComp = ecs.get<Engine::ECS::Component::Logic2D::CollisionComponent>(e);

		glm::vec2 moveDir(0.0f);

		glm::vec3 camFront3D = renderContext.cameraFront;
		glm::vec2 camForwrdXZ = glm::normalize(glm::vec2(camFront3D.x, camFront3D.z));

		glm::vec3 camRight3D = renderContext.cameraRight;
		glm::vec2 camRightXZ = glm::normalize(glm::vec2(camRight3D.x, camRight3D.z));

		// Movement based on relative coordinates
		// 相対座標を基準に移動
		if (input.isPressed(InputAction::MoveForward))
			moveDir += camForwrdXZ;
		if (input.isPressed(InputAction::MoveBackward))
			moveDir -= camForwrdXZ;
		if (input.isPressed(InputAction::MoveRight))
			moveDir += camRightXZ;
		if (input.isPressed(InputAction::MoveLeft))
			moveDir -= camRightXZ;

		if (glm::length(moveDir) > 0.0f)
		{
			moveDir = glm::normalize(moveDir);
			logic.positionXZ += moveDir * deltaTime * 5.0f; // 移動速度

			// ここが問題だと思う(責務分離)
			// ここが問題だと思う(責務分離)
			// ここが問題だと思う(責務分離)
			// ここが問題だと思う(責務分離)
			// ここが問題だと思う(責務分離)
			// collision update
			//if (collisionComp.collider.type == Engine::ECS::Component::Logic2D::ColliderType::Circle2D)
			//{
			//	collisionComp.collider.circle2D.center = logic.positionXZ;
			//}
			// ここで，(local)centerを更新するのはよくない
			if (collisionComp.collider.IsCircle2D())
			{
				collisionComp.collider.AsCircle2D().center = logic.positionXZ;
			}
		}


		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp", input.screenMousePosition);
		glm::vec2 dir = Game::Utils::GetLogicDirectionFromCursorToTarget(rawInput.mousePosition, renderContext, logic.positionXZ);

		// for testing
		//GameUtils::SpatialTransform::ProjectScreenToLogicXZPlane(input.screenMousePosition, renderContext);

		// vec2(0.0f)は正規化できません。(isnan=> Not a Number)
		if (glm::length(dir) > 0.0001f && !glm::any(glm::isnan(dir)))
		{
			logic.front = glm::normalize(dir);
			logic.rotation = logic.GetRotationYFromFrontVector();//radians / -Zが正面(右手系:反時計回りが正)

			// rightベクトルもfrontから再計算
			logic.UpdateRightFromFront();
		}
		else
		{
			logic.front = CanonicalDefaults::kCanonicalForwardXZ;
		}



		// log for debugging
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(position)", logic.positionXZ);
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(front)", logic.front);
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(right)", logic.right);
		// std::cout << "[PlayerCharacterControlSystem.cpp] rotation " << logic.rotation << std::endl;
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(rotation)", {logic.rotation, 0});
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(circleCenter)", collisionComp.collider.circle2D.center);
		// std::cout << "RADIUS" << collisionComp.collider.circle2D.radius << std::endl;


		break;
	}
}


void Game::Input::Player::Update(Engine::ECS::EntityMgr& ecs, InputState& input, float deltaTime, Engine::Graphics::Render::RenderContext& renderContext)
{
	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Tags::PlayerControllerComponent, Engine::ECS::Component::Logic2D::Logic2DTransformComponent>())
	{
		auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e);
		auto& collisionComp = ecs.get<Engine::ECS::Component::Logic2D::CollisionComponent>(e);

		glm::vec2 moveDir(0.0f);

		glm::vec3 camFront3D = renderContext.cameraFront;
		glm::vec2 camForwrdXZ = glm::normalize(glm::vec2(camFront3D.x, camFront3D.z));

		glm::vec3 camRight3D = renderContext.cameraRight;
		glm::vec2 camRightXZ = glm::normalize(glm::vec2(camRight3D.x, camRight3D.z));

		// Movement based on relative coordinates
		// 相対座標を基準に移動
		if (input.isPressed(InputAction::MoveForward))
			moveDir += camForwrdXZ;
		if (input.isPressed(InputAction::MoveBackward))
			moveDir -= camForwrdXZ;
		if (input.isPressed(InputAction::MoveRight))
			moveDir += camRightXZ;
		if (input.isPressed(InputAction::MoveLeft))
			moveDir -= camRightXZ;

		if (glm::length(moveDir) > 0.0f)
		{
			moveDir = glm::normalize(moveDir);
			logic.positionXZ += moveDir * deltaTime * 5.0f; // 移動速度

			//// collision update
			//if (collisionComp.collider.type == Engine::ECS::Component::Logic2D::ColliderType::Circle2D)
			//{
			//	collisionComp.collider.circle2D.center = logic.positionXZ;
			//}

			// ここで，(local)centerを更新するのはよくない
			if (collisionComp.collider.IsCircle2D())
			{
				collisionComp.collider.AsCircle2D().center = logic.positionXZ;
			}
		}


		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp", input.screenMousePosition);

		glm::vec2 dir = Game::Utils::GetLogicDirectionFromCursorToTarget(input.screenMousePosition, renderContext, logic.positionXZ);

		// for testing
		//GameUtils::SpatialTransform::ProjectScreenToLogicXZPlane(input.screenMousePosition, renderContext);

		// vec2(0.0f)は正規化できません。(isnan=> Not a Number)
		if (glm::length(dir) > 0.0001f && !glm::any(glm::isnan(dir)))
		{
			logic.front = glm::normalize(dir);
			logic.rotation = logic.GetRotationYFromFrontVector();


			// rightベクトルもfrontから再計算
			logic.UpdateRightFromFront();

		}
		else
		{
			logic.front = glm::vec2(0.0f, 1.0f);
		}



		// log for debugging
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(position)", logic.positionXZ);
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(front)", logic.front);
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(rotation)", {logic.rotation, 0});
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(circleCenter)", collisionComp.collider.circle2D.center);
		// std::cout << "RADIUS" << collisionComp.collider.circle2D.radius << std::endl;


		break;
	}
}


// Input for the player's character
void Game::Input::Player::Update(Engine::ECS::EntityMgr& ecs, InputState& input, float deltaTime)
{
	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Tags::PlayerControllerComponent, Engine::ECS::Component::Logic2D::Logic2DTransformComponent>())
	{
		auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e);

		glm::vec2 moveDir(0.0f);

		// Movement based on relative coordinates
		// 相対座標を基準に移動
		if (input.isPressed(InputAction::MoveForward))
			moveDir += logic.front;
		if (input.isPressed(InputAction::MoveBackward))
			moveDir -= logic.front;
		if (input.isPressed(InputAction::MoveRight))
			moveDir += logic.right;
		if (input.isPressed(InputAction::MoveLeft))
			moveDir -= logic.right;

		if (glm::length(moveDir) > 0.0f)
		{
			moveDir = glm::normalize(moveDir);
			logic.positionXZ += moveDir * deltaTime * 3.0f; // 移動速度
		}


		logic.UpdateDirectionFromRotation();

		// position log for debugging
		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp", logic.positionXZ);
		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp", logic.front);

		break;
	}
}