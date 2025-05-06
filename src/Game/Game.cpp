#include "Game.h"

// std system
#include <iostream>

//Entity
#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

//Components
#include "Core/ECS/Component/TransformComponent.h"

// Window
#include "Core/Window/WindowManager.h"

// physics
#include "Core/Physics/CollisionUtils.h"
#include "DataTypes/ColliderType.h"

// Graphic
#include "Graphics/Renderer/Shader.h"
#include "Graphics/Renderer/RenderSystem.h"
#include "Graphics/Model/AssimpImporter.h"

//Game/Actor
#include "Game/Actor/CameraActor.h"
#include "Game/Actor/PlayerCharacterActor.h"
#include "Game/Actor/FollowCameraActor.h"
#include "Game/Actor/MouseCursorActor.h"
#include "Game/Actor/Map/TestBaseTerrainActor.h"
// Game/Actor/Map
#include "Game/Actor/Map/TileMapActor.h"

// Game/Camera
#include "Game/Camera/CameraFollowSystem.h"

// Game/Input
#include "Game/Input/CameraControlSystem.h"
#include "Game/Input/PlayerCharacterControlSystem.h"
#include "Game/Input/MouseCursorUpdateSystem.h"


// Game/Sync
#include "Game/Sync/SyncLogicToTransformSystem.h"

// Debug
#include "Debug/DebugSystems/LogicDebugDrawSystem.h"

// Test
//#include "Test/TriangleActor.h"
//#include "Test/Test3DModel.h"

// コンストラクタ
Game::Game()
	: mIsRunning(true)
	, mShader(nullptr)
	, mRenderContext()
	, windowWidth(1280)
	, windowHeight(720)
{

}

// 終了処理
void Game::Shutdown()
{
	unloadData();

	// Destroy all components
	mEcs.Clear();

	delete mShader;
	mShader = nullptr;

	WindowManager::Shutdown();

	std::cout << "\n[Game.cpp (Shutdown)]: The application shut down successfully." << std::endl;
}

// 初期化
bool Game::Initialize()
{
	//if (!glfwInit())
	//{
	//	std::cerr << "[Game.cpp (Initialize)]: Failed to initialize GLFW" << std::endl;
	//	return false;
	//}

	//mWindow = glfwCreateWindow(mWindow_Width, mWindow_Height,"GameWindow", nullptr, nullptr);
	//if (!mWindow)
	//{
	//	std::cerr << "[Game.cpp (Initialize)]: Failed to initialize GLFW window" << std::endl;
	//	return false;
	//}

	//glfwMakeContextCurrent(mWindow);

	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	//{
	//	std::cerr << "[Game.cpp (Initialize)]: Failed to initialize GLAD" << std::endl;
	//	return false;
	//}

	//glViewport(0, 0, mWindow_Width, mWindow_Height);
	
	if (!WindowManager::Initialize(windowWidth, windowHeight, "Game"))
	{
		std::cerr << "[Game.cpp]: Failed to Initialize WindowManager" << std::endl;
		return false;
	}
	
	// WindowManager::CaptureMouse();

	glEnable(GL_DEPTH_TEST);


	// ShaderInit
	mShader = new Shader("shaders/basic.vertex.glsl", "shaders/basic.fragment.glsl");


	// Log
	std::cout << "[Game.cpp (Initialize)]: Application initialization completed successfully" << std::endl;


	loadData();

	return true;
}

void Game::RunLoop()
{
	while (!glfwWindowShouldClose(WindowManager::GetWindow()))
	{

		updateGameLogics();

		generateOutputs();

		glfwPollEvents();


		// Newline output for console debugging
		// std::cout << std::endl;
	}
}

void Game::updateGameLogics()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	mDeltaTime = currentFrame - mLastFrame;
	mLastFrame = currentFrame;

	// 入力状態マップの更新
	mInputMapping.update(WindowManager::GetWindow(), mInputState);

	// characterの移動
	PlayerCharacterControlSystem::Update(mEcs, mInputState, mDeltaTime, mRenderContext);
	// PlayerCharacterControlSystem::Update(mEcs, mInputState, mDeltaTime);

	// 2D (Logic)-> 3D (Drawing)
	SyncLogicToTransformSystem::Apply2DToTransform(mEcs, mDeltaTime);

	// カメラ
	CameraFollowSystem::Update(mEcs, mDeltaTime);
	// GameSystemInput::UpdateCamera(mEcs, mInputState, mDeltaTime);
	
	MouseCursorUpdateSystem::Update(mEcs, mInputState, mRenderContext);

}

void Game::generateOutputs()
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// An algorithm is needed to set the shader for each object.
	//RenderSystem::RenderSystem(mEcs, *mShader, WindowManager::GetAspect());
	RenderSystem::RenderSystem(mEcs, *mShader, WindowManager::GetAspect(), mRenderContext);


	// draw for debugging
	LogicDebugDrawSystem::Draw(mEcs, mRenderContext);


	//
	glfwSwapBuffers(WindowManager::GetWindow());
}

void Game::loadData()
{

	//TriangleActor tri = TriangleActor(mEcs);

	//Test3DModel test3d = Test3DModel(mEcs, mShader);

	PlayerCharacter player = PlayerCharacter(mEcs, mShader);

	FollowCameraActor followCam = FollowCameraActor(mEcs);

	MouseCursorActor mouseCursor = MouseCursorActor(mEcs);

	// TileMapActor tilemap = TileMapActor(mEcs);

	
	TestBaseTerrainActor testTerrainMap = TestBaseTerrainActor(mEcs, mShader);
	
	
	// CameraActor camActor = CameraActor(mEcs);

	//Collider a, c;
	//c.type = ColliderType::Box2D;
	//a.type = ColliderType::Box2D;
	//c.box2D = Box2D{ glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	//a.box2D = Box2D { glm::vec2(3.0f, 0.0f) , glm::vec2(1.0f,1.0f) };

	//bool hit = CollisionUtils::intersectBox2D(a.box2D, c.box2D);

	//std::cout << "Result: " << std::boolalpha << hit << std::endl;

	std::cout << "[Game.cpp]: Data loading completed successfully." << std::endl;
}

void Game::unloadData()
{

}


