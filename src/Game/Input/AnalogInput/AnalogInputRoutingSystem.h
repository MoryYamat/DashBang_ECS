//

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/InputManager/RawInputState.h"
#include "Engine/Graphics/Renderer/RenderContext.h"




namespace Game::Input::Analog
{
	void RouteAnalogInput(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput, const Engine::Graphics::Render::RenderContext& renderContext);
}