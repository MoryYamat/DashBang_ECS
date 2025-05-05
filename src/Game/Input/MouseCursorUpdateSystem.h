// A function that updates the mouse cursor state.

#pragma once

#include "Core/ECS/EntityManager.h"

#include "Game/Input/InputState.h"

#include "Graphics/Renderer/RenderContext.h"

namespace MouseCursorUpdateSystem
{
	void Update(ECS& ecs, InputState& input, RenderContext renderContext);
}