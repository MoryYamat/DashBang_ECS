// Game Loop
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/ECS/EntityManager.h"


class Game
{
public:

	Game();
	
	bool Initialize();

	void Shutdown();

	void RunLoop();


private:
	// loop flag => ãè‚Ég‚¤•û–@
	bool mIsRunning;

	int mWindow_Width, mWindow_Height;
	float mAspect;

	class ECS mEcs;

	GLFWwindow* mWindow;

	class Shader* mShader;

	void loadData();

	void unloadData();
};