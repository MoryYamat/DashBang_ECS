#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/Window/Public/WindowFwd.hpp"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"

namespace Game::Layer
{
	struct DrawLayerFeature
	{
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader& shader
			, Engine::Window::Window& window, Engine::Graphics::RenderContext& renderCtx);
	};
}