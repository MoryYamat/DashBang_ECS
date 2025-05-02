// player position control system
#pragma once

#include "Core/ECS/EntityManager.h"
#include "Game/Input/InputState.h"

namespace PlayerCharacterControlSystem
{
	void Update(ECS& ecs, InputState& input, float deltaTime);
};