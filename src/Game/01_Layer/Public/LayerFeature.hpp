#pragma once

#include "Engine/InputManager/Public/InputFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/Window/Public/WindowFwd.hpp"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"

namespace Game::Layer
{
	class LocomotionAnimLayerFeature
	{
	public:
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	};



	// 初期化フェーズ
	class InitializeLayerFeature
	{
	public:
		// 初期化順に注意
		// 遅らせ初期化

		static void DelayedInitialization(Engine::WorldSystem::Core::WorldCtx& ctx);
	};

	struct InputLayer
	{
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx,
			const Engine::Input::RawInputState& raw, const Engine::Graphics::RenderContext& renderCtx);
	};

	class IntentLayerFeature
	{
	public:
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	};

	class LogicLayerFeature
	{
	public:
		
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	};

	class ResolverLayerFeature
	{
	public:
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	};

	class StateLayerFeature
	{
	public:
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	};

	struct DrawLayerFeature
	{
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader& shader
			, Engine::Window::Window& window, Engine::Graphics::RenderContext& renderCtx);
	};
}

namespace Game::Layer::Debug
{
	class DebugLayerFeature
	{
	public:
		// static void Update(Engine::ECS::EntityMgr& ecs);
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}