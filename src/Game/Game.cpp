#include "Game.h"

// std system
#include <iostream>

//Entity
#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

//Components
#include "Core/ECS/Component/TransformComponent.h"

// Graphic
#include "Graphics/Renderer/Shader.h"
#include "Graphics/Renderer/RendererSystem.h"
#include "Graphics/Model/AssimpImporter.h"

//Game/Actor
#include "Game/Actor/CameraActor.h"

// Game/Input
#include "Game/Input/CameraControlSystem.h"

// Test
#include "Test/TriangleActor.h"
#include "Test/Test3DModel.h"

// �R���X�g���N�^
Game::Game()
	: mIsRunning(true)
	, mWindow(nullptr)
	, mShader(nullptr)
	, mWindow_Width(1280)
	, mWindow_Height(720)
	, mAspect(0.0f)
{

}

// �I������
void Game::Shutdown()
{
	unloadData();

	delete mShader;
	mShader = nullptr;

	glfwDestroyWindow(mWindow);
	glfwTerminate();

	std::cout << "[Game.cpp (Shutdown)]: The application shut down successfully." << std::endl;
}

// ������
bool Game::Initialize()
{
	if (!glfwInit())
	{
		std::cerr << "[Game.cpp (Initialize)]: Failed to initialize GLFW" << std::endl;
		return false;
	}

	mWindow = glfwCreateWindow(mWindow_Width, mWindow_Height,"GameWindow", nullptr, nullptr);
	if (!mWindow)
	{
		std::cerr << "[Game.cpp (Initialize)]: Failed to initialize GLFW window" << std::endl;
		return false;
	}

	glfwMakeContextCurrent(mWindow);

	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "[Game.cpp (Initialize)]: Failed to initialize GLAD" << std::endl;
		return false;
	}

	glViewport(0, 0, mWindow_Width, mWindow_Height);
	glEnable(GL_DEPTH_TEST);

	mShader = new Shader("shaders/basic.vertex.glsl", "shaders/basic.fragment.glsl");

	mAspect = mWindow_Width / mWindow_Height;

	std::cout << "[Game.cpp (Initialize)]: Application initialization completed successfully" << std::endl;


	loadData();

	return true;
}

void Game::RunLoop()
{
	while (!glfwWindowShouldClose(mWindow))
	{

		updateGameLogics();

		generateOutputs();

		glfwPollEvents();
	}
}

void Game::updateGameLogics()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	mDeltaTime = currentFrame - mLastFrame;
	mLastFrame = currentFrame;

	// ���͏�ԃ}�b�v�̍X�V
	mInputMapping.update(mWindow, mInputState);

	// �J����
	GameSystemInput::UpdateCamera(mEcs, mInputState, mDeltaTime);
}

void Game::generateOutputs()
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// An algorithm is needed to set the shader for each object.
	RenderSystem::RenderSystem(mEcs, *mShader, mAspect);

	glfwSwapBuffers(mWindow);
}

void Game::loadData()
{

	//TriangleActor tri = TriangleActor(mEcs);

	Test3DModel test3d = Test3DModel(mEcs, mShader);

	CameraActor camActor = CameraActor(mEcs);

	std::cout << "[Game.cpp]: Data loading completed successfully." << std::endl;
}

void Game::unloadData()
{
}