// Game Loop
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ======================= Common =======================
#include "Common/GameNameSpaceDecl.h"
#include "Common/EngineNamespaceDecl.h"


// ======================= Engine =======================
#include "Engine/ECS/EntityManager.h"

// Graphics
#include "Engine/Graphics/Renderer/Shader.h"
#include "Engine/Graphics/Renderer/RenderContext.h"

#include "Engine/Window/Window.h"

// Input
#include "Engine/InputManager/InputManager.h"
#include "Engine/InputManager/RawInputState.h"



// ======================= Game =======================
#include "Game/Collision/Data/CollisionResultStorage.h"

#include "Game/Input/InputState.h"
#include "Game/Input/InputMapping.h"

// skill
#include "Game/Combat/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Skill/Trigger/SkillInputMap.h"



namespace GameApp
{
	class GameApp
	{
	public:

		GameApp();

		bool Initialize();

		void Shutdown();

		void RunLoop();


	// できるだけ前方宣言を使用できる構造にする必要がある(構造破綻を防止するため)
	// できるだけ前方宣言を使用できる構造にする必要がある(構造破綻を防止するため)
	// できるだけ前方宣言を使用できる構造にする必要がある(構造破綻を防止するため)
	// できるだけ前方宣言を使用できる構造にする必要がある(構造破綻を防止するため)
	private:
		// loop flag => 上手に使う方法
		bool mIsRunning;

		int windowWidth, windowHeight;

		// ecs
		eNsECS::EntityMgr mECS;

		// shader
		eNsGfxRender::Shader* mShader;// new するので完全な定義が必要
		eNsGfxRender::RenderContext mRenderContext;

		Engine::Window::Window mWindow;

		eNsInput::InputManager* mInputManager;

		// input states
		gNsInput::InputMapping mInputMapping;

		// collision detection results
		gNsCollData::CollisionResultStorage mCollisionResults;

		// skill 
		// gNsSkillData::SkillDatabase mSkillDatabase;
		gNsSkillTrigger::SkillInputMap mSkillInputMap;

		// float mDeltaTime = 0.0f;
		float mLastFrame = 0.0f;

		void updateGameLogics();

		void generateOutputs();

		void loadData();

		void unloadData();

		void spawnAllActors();

		void RunInitializationPhase();

		void updateContext();

		void InitializeInputMapping();

		void InitializeSkills();

		void InitializeSkillMappings();

		// ECSのグローバルリソースとして保持する基準
		// ：どのエンティティやシステムからも参照される
		// ：頻繁に変更されず，不変／静的に扱える => SkillInputMap, InputMapping
		// ：複数のSystemが依存しているが，Componentではない => CollisionResultStorage(状態ではなくバッファ)
		// ：状態を持つ必要がなく，ECSの生存管理に関係しない
		void InitializeGlobalResouces();
	};
}


