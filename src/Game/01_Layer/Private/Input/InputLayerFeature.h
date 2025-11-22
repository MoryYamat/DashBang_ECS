#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/InputManager/Public/InputFwd.hpp"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"

namespace Game::Layer
{
	struct InputLayer
	{
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx, 
			const Engine::Input::RawInputState& raw, const Engine::Graphics::RenderContext& renderCtx);
	};
}