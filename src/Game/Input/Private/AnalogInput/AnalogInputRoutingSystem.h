//

#pragma once

#include "Engine/ECS/EntityManager.h"

//#include "Engine/InputManager/RawInputState.h"
#include "Engine/InputManager/Public/InputFwd.hpp"
// #include "Engine/Graphics/Renderer/RenderContext.h"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"


#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Input::Analog
{
	void RouteAnalogInput(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput, const Engine::Graphics::Render::RenderContext& renderContext);
	
	void RouteAnalogInput(Engine::WorldSystem::Core::WorldCtx& ctx);
}