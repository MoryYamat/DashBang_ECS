#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Engine/InputManager/Public/InputFwd.hpp"

#include "Engine/Graphics/Public/GraphicsFwd.hpp"

namespace Game::Input
{
	void InitAllInputSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	// void SetInputMapping(Engine::WorldSystem::Core::WorldCtx& ctx);

	void InputRouterSystem(Engine::WorldSystem::Core::WorldCtx& ctx, const Engine::Input::RawInputState& raw);

	void RouteAnalogInput(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Input::RawInputState& rawInput, const Engine::Graphics::RenderContext& renderContext);


}