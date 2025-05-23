#include "PlayerCharacterControlSystem.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Core/InputManager/InputManager.h"

// collision
#include "Core/ECS/Component/Collision/CollisionComponent.h"
#include "Core/ECS/Component/Collision/ColliderType.h"

#include "Core/ECS/Component/InputActionComponent.h"

#include "Debug/DebugUtils.h"

#include "Game/Utils/SpatialTransformUtils.h"

#include <glm/ext/matrix_projection.hpp>

#include <cmath>
#include <iostream>

// raw Input と Game Input の分離後
void PlayerCharacterControlSystem::Update(ECS& ecs, const RawInputState& rawInput, RenderContext& renderContext, float deltaTime)
{
	InputActionComponent input;
	for (Entity e : ecs.view<PlayerControllerComponent, InputActionComponent>())
	{
		input = ecs.get<InputActionComponent>(e);
		break;
	}

	for (Entity e : ecs.view<PlayerControllerComponent, Logic2DTransformComponent>())
	{
		auto& logic = ecs.get<Logic2DTransformComponent>(e);
		auto& collisionComp = ecs.get<CollisionComponent>(e);

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

			// collision update
			if (collisionComp.collider.type == ColliderType::Circle2D)
			{
				collisionComp.collider.circle2D.center = logic.positionXZ;
			}
		}


		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp", input.screenMousePosition);
		glm::vec2 dir = GameUtils::SpatialTransform::GetLogicDirectionFromCursorToTarget(rawInput.mousePosition, renderContext, logic.positionXZ);

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
		std::cout << "[PlayerCharacterControlSystem.cpp] rotation " << logic.rotation << std::endl;
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(rotation)", {logic.rotation, 0});
		// DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(circleCenter)", collisionComp.collider.circle2D.center);
		// std::cout << "RADIUS" << collisionComp.collider.circle2D.radius << std::endl;


		break;
	}
}


void PlayerCharacterControlSystem::Update(ECS& ecs, InputState& input, float deltaTime, RenderContext& renderContext)
{
	for (Entity e : ecs.view<PlayerControllerComponent, Logic2DTransformComponent>())
	{
		auto& logic = ecs.get<Logic2DTransformComponent>(e);
		auto& collisionComp = ecs.get<CollisionComponent>(e);

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

			// collision update
			if (collisionComp.collider.type == ColliderType::Circle2D)
			{
				collisionComp.collider.circle2D.center = logic.positionXZ;
			}
		}


		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp", input.screenMousePosition);

		glm::vec2 dir = GameUtils::SpatialTransform::GetLogicDirectionFromCursorToTarget(input.screenMousePosition, renderContext, logic.positionXZ);

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
void PlayerCharacterControlSystem::Update(ECS& ecs, InputState& input, float deltaTime)
{
	for (Entity e : ecs.view<PlayerControllerComponent, Logic2DTransformComponent>())
	{
		auto& logic = ecs.get<Logic2DTransformComponent>(e);

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