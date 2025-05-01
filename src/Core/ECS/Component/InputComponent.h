// Input Component

#pragma once

#include "Game/Input/InputState.h"

struct InputComponent
{
	InputState* inputState = nullptr;// Shared View Only(‹¤—LQÆ‚Ì‚İ)

	float moveSpeed = 5.0f;
};