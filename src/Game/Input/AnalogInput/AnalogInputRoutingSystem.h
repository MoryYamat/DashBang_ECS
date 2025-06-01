//

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/InputManager/RawInputState.h"
#include "Engine/Graphics/Renderer/RenderContext.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Input::Analog
{
	void RouteAnalogInput(eNsECS::EntityMgr& ecs, const eNsInput::RawInputState& rawInput, const eNsGfxRender::RenderContext& renderContext);
}