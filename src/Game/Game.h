// Game Loop
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/ECS/EntityManager.h"

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

	int mWindow_Width, mWindow_Height;
	float mAspect;

	class ECS mEcs;

	GLFWwindow* mWindow;

	class Shader* mShader;

	class InputState mInputState;
	class InputMapping mInputMapping;

	float mDeltaTime = 0.0f;
	float mLastFrame = 0.0f;

	void updateGameLogics();

	void generateOutputs();

	void loadData();

	void unloadData();
};