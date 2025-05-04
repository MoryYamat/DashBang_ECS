#include "PlayerCharacterControlSystem.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/PlayerControllerComponent.h"

#include "Debug/DebugUtils.h"

#include "Game/Utils/SpatialTransformUtils.h"


#include <glm/ext/matrix_projection.hpp>

#include <cmath>
#include <iostream>

// Input for the player's character
void PlayerCharacterControlSystem::Update(ECS& ecs, InputState& input, float deltaTime)
{
	for (Entity e : ecs.view<PlayerControllerComponent, Logic2DTransformComponent>())
	{
		auto& logic = ecs.get<Logic2DTransformComponent>(e);

		glm::vec2 moveDir(0.0f);

		// Movement based on relative coordinates
		// ���΍��W����Ɉړ�
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
			logic.positionXZ += moveDir * deltaTime * 3.0f; // �ړ����x
		}


		logic.UpdateDirectionFromRotation();

		// position log for debugging
		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp", logic.positionXZ);
		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp", logic.front);

		break;
	}	
}


void PlayerCharacterControlSystem::Update(ECS& ecs, InputState& input, float deltaTime, RenderContext& renderContext)
{
	for (Entity e : ecs.view<PlayerControllerComponent, Logic2DTransformComponent>())
	{
		auto& logic = ecs.get<Logic2DTransformComponent>(e);

		glm::vec2 moveDir(0.0f);

		// Movement based on relative coordinates
		// ���΍��W����Ɉړ�
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
			logic.positionXZ += moveDir * deltaTime * 3.0f; // �ړ����x
		}




		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp", input.screenMousePosition);

		glm::vec2 dir = GameUtils::SpatialTransform::MouseTo2DLogicConverter(input.screenMousePosition,
			renderContext.view,
			renderContext.projection,
			glm::vec4(0, 0, renderContext.viewportSize.x, renderContext.viewportSize.y),
			logic.positionXZ
		);

		// vec2(0.0f)�͐��K���ł��܂���B(isnan=> Not a Number)
		if (glm::length(dir) > 0.0001f && !glm::any(glm::isnan(dir)))
		{
			logic.front = glm::normalize(dir);
			logic.rotation = logic.GetRotationYFromFrontVector();
		}
		else
		{
			logic.front = glm::vec2(0.0f, 1.0f);
		}



		// position log for debugging
		DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(position)", logic.positionXZ);
		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(front)", logic.front);
		//DebugUtils::LogVector("PlayerCharacterControlSystem.cpp(rotation)", {logic.rotation, 0});

		break;
	}
}


