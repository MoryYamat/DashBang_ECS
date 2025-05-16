// A function that updates the mouse cursor state.

#pragma once

#include "Core/ECS/EntityManager.h"

#include "Game/Input/InputState.h"

#include "Core/InputManager/RawInputState.h"

#include "Graphics/Renderer/RenderContext.h"

namespace MouseCursorUpdateSystem
{
	void Update(ECS& ecs, const RawInputState& rawInput, RenderContext& renderContext);

	void Update(ECS& ecs, InputState& input, RenderContext renderContext);
}