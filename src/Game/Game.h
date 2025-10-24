// Game Loop
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ======================= Common =======================




// ======================= Engine =======================
#include "Engine/ECS/EntityManager.h"

// Graphics
#include "Engine/Graphics/Private/Renderer/Shader.h"
#include "Engine/Graphics/Private/Renderer/RenderContext.h"

#include "Engine/Window/Private/Window.h"

// Input
#include "Engine/InputManager/Private/InputManager.h"

#include "Game/Collision/Private/Data/CollisionResultStorage.h"

// 
#include "Game/Common/Private/AppContext.h"

// World
// #include "Engine/WorldSystem/Private/Core/World.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

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

	// できるだけ前方宣言を使用できる構造にする必要がある(構造破綻を防止するため)
	// できるだけ前方宣言を使用できる構造にする必要がある(構造破綻を防止するため)
	// できるだけ前方宣言を使用できる構造にする必要がある(構造破綻を防止するため)
	// できるだけ前方宣言を使用できる構造にする必要がある(構造破綻を防止するため)
	private:

		std::unique_ptr<Engine::WorldSystem::Core::World> world;

		std::unique_ptr<Engine::Window::Window> mWindow;
		std::unique_ptr<Engine::Input::InputManager> mInput;
		std::unique_ptr<Engine::Graphics::Render::Shader> mShader;
		Engine::ECS::EntityMgr mECS;
		Engine::Graphics::Render::RenderContext mRenderCtx;

		// 参照の束 (所有しない)
		Game::Common::AppContext mCtx;

		float mLastFrame = 0.0f;

		// loop flag => 上手に使う方法
		bool mIsRunning;

		int windowWidth, windowHeight;

		// 
		//// ecs
		//Engine::ECS::EntityMgr mECS;

		//// shader
		//Engine::Graphics::Render::Shader* mShader;// new するので完全な定義が必要
		//Engine::Graphics::Render::RenderContext mRenderContext;

		//Engine::Window::Window mWindow;

		//Engine::Input::InputManager* mInputManager;

		Game::Collision::Data::CollisionResultStorage mCollisionResults;



		// float mDeltaTime = 0.0f;

		void updateGameLogics();
		void updateGameLogics(Engine::WorldSystem::Core::WorldCtx& ctx);

		void generateOutputs();
		void generateOutputs(const Engine::WorldSystem::Core::WorldCtx& ctx);

		void Update(Engine::WorldSystem::Core::WorldCtx& ctx, float realDt);

		void loadData();
	
		void unloadData();

		void spawnAllActors();

		void RunInitializationPhase();

		void updateContext();

		// ECSのグローバルリソースとして保持する基準
		// ：どのエンティティやシステムからも参照される
		// ：頻繁に変更されず，不変／静的に扱える => SkillInputMap, InputMapping
		// ：複数のSystemが依存しているが，Componentではない => CollisionResultStorage(状態ではなくバッファ)
		// ：状態を持つ必要がなく，ECSの生存管理に関係しない
		void InitializeGlobalResouces();
	};
}


