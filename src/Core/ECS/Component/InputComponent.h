// Input Component

#pragma once

#include "Game/Input/InputState.h"

struct InputComponent
{
	InputState* inputState = nullptr;// Shared View Only(���L�Q�Ƃ̂�)

	float moveSpeed = 5.0f;
};