#include "PlayerCharacterControlSystem.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/PlayerControllerComponent.h"

#include "Debug/DebugUtils.h"

#include <iostream>

// Input for the player's character
void PlayerCharacterControlSystem::Update(ECS& ecs, InputState& input, float deltaTime)
{
	for (Entity e : ecs.view<PlayerControllerComponent, Logic2DTransformComponent>())
	{
		auto& logic = ecs.get<Logic2DTransformComponent>(e);

		glm::vec2 moveDir(0.0f);

		// Movement based on relative coordinates
		// ‘Š‘ÎÀ•W‚ðŠî€‚ÉˆÚ“®
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
			logic.positionXZ += moveDir * deltaTime * 3.0f; // ˆÚ“®‘¬“x
		}

		logic.UpdateDirectionFromRotation();


		// position log for debugging
		//DebugUtils::LogPosition("PlayerCharacterControlSystem.cpp", logic.positionXZ);

		break;
	}

	
}
