// player position control system
#pragma once

#include "Core/ECS/EntityManager.h"
#include "Game/Input/InputState.h"

class PlayerCharacterControlSystem
{
public:

	void Update(ECS& ecs, InputState& input, float deltaTime);
};