// A function that updates the mouse cursor state.

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Input/InputState.h"

#include "Engine/InputManager/RawInputState.h"

#include "Engine/Graphics/Renderer/RenderContext.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Input::Analog
{
	void Update(eNsECS::EntityMgr& ecs, const eNsInput::RawInputState& rawInput, eNsGfxRender::RenderContext& renderContext);

	void Update(eNsECS::EntityMgr& ecs, InputState& input, eNsGfxRender::RenderContext renderContext);
}