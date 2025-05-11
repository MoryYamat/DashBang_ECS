// Game Loop
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/ECS/EntityManager.h"

#include "Graphics/Renderer/RenderContext.h"

#include "Game/CollisionLogic/Data/CollisionResultStorage.h"

#include "Game/Input/InputState.h"
#include "Game/Input/InputMapping.h"


class Game
{
public:

	Game();
	
	bool Initialize();

	void Shutdown();

	void RunLoop();


private:
	// loop flag => è„éËÇ…égÇ§ï˚ñ@
	bool mIsRunning;

	int windowWidth, windowHeight;

	// ecs
	class ECS mEcs;

	// shader
	class Shader* mShader;
	RenderContext mRenderContext;

	// input states
	class InputState mInputState;
	class InputMapping mInputMapping;

	// collision detection results
	CollisionResultStorage mCollisionResults;

	float mDeltaTime = 0.0f;
	float mLastFrame = 0.0f;

	void updateGameLogics();

	void generateOutputs();

	void loadData();

	void unloadData();

};

