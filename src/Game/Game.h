// Game Loop
#pragma once

// ======================= Common =======================




// ======================= Engine =======================
#include "Engine/ECS/EntityManager.h"

// Audio
#include "Engine/Audio/Public/AudioFwd.hpp"

// Graphics
#include "Engine/Graphics/Private/Renderer/Shader.h"
#include "Engine/Graphics/Private/Renderer/RenderContext.h"
#include "Engine/Graphics/Public/Types.hpp"

// Input
#include "Engine/InputManager/Public/InputFwd.hpp"

// window
#include "Engine/Window/Public/WindowFwd.hpp"

// World
// #include "Engine/WorldSystem/Private/Core/World.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

// Debug
#include "Engine/Debug/Public/DebugFWD.hpp"

#include <memory>

namespace GameApp
{
	class GameApp
	{
	public:

		GameApp();
		~GameApp();

		bool Initialize();
		void Shutdown();
		void RunLoop();


	private:

		std::unique_ptr<::Engine::WorldSystem::Core::World> world_;
		std::unique_ptr<::Engine::Window::Window> window_;
		std::unique_ptr<::Engine::Input::InputManager> input_;
		std::unique_ptr<::Engine::Graphics::Shader> shader_;
		std::unique_ptr<::Engine::Debug::DebugLineRenderer> debugLineRenderer_;
		::Engine::Graphics::RenderContext renderCtx_;

		std::unique_ptr<::Engine::Audio::AudioSystem> audioSys_;

		// 参照の束 (所有しない)
		// Game::Common::AppContext mCtx;

		float lastFrame_ = 0.0f;

		// loop flag => 上手に使う方法
		bool isRunning_;

		int windowWidth_, windowHeight_;

		// 
		//// ecs
		//Engine::ECS::EntityMgr mECS;

		//// shader
		//Engine::Graphics::Render::Shader* mShader;// new するので完全な定義が必要
		//Engine::Graphics::Render::RenderContext mRenderContext;

		//Engine::Window::Window mWindow;

		//Engine::Input::InputManager* mInputManager;

		// Game::Collision::Data::CollisionResultStorage mCollisionResults;



		// float mDeltaTime = 0.0f;

		// void updateGameLogics();
		void updateGameLogics(::Engine::WorldSystem::Core::WorldCtx& ctx);

		void generateOutputs(::Engine::WorldSystem::Core::WorldCtx& ctx);

		void Update(::Engine::WorldSystem::Core::WorldCtx& ctx, float realDt);

		void loadData();
		void loadData(::Engine::WorldSystem::Core::WorldCtx& ctx);
	
		void unloadData();

		void spawnAllActors(::Engine::WorldSystem::Core::WorldCtx& ctx);

		void RunInitializationPhase(::Engine::WorldSystem::Core::WorldCtx& ctx);

		void updateContext();

		// ECSのグローバルリソースとして保持する基準
		// ：どのエンティティやシステムからも参照される
		// ：頻繁に変更されず，不変／静的に扱える => SkillInputMap, InputMapping
		// ：複数のSystemが依存しているが，Componentではない => CollisionResultStorage(状態ではなくバッファ)
		// ：状態を持つ必要がなく，ECSの生存管理に関係しない

	};
}


