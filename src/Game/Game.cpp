#include "Game.h"

// std system
#include <iostream>

//Entity
#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Meta/InitComponent/FollowCameraInit.h"

//Components
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/TileMapComponent.h"

// Init Components (Must be included for specialization)
#include "Core/ECS/Meta/InitComponent/InitDispatcher.h"
#include "Core/ECS/Meta/InitComponent/TileMapInit.h"
#include "Core/ECS/Meta/InitComponent/FollowCameraInit.h"

// Window

// physics
#include "Core/Physics/CollisionUtils.h"
#include "Core/ECS/Component/Collision/ColliderType.h"

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
#include "Game/Actor/Map/Obstacle/TestRockActor.h"

// Game/Camera
#include "Game/Camera/CameraFollowSystem.h"

// Game/Input
#include "Game/Camera/CameraControlSystem.h"
#include "Game/Input/PlayerCharacterControlSystem.h"
#include "Game/Input/MouseCursorUpdateSystem.h"
#include "Game/Input/InputRouterSystem.h"

// Game Init
#include "Game/Init/InitTileMap/InitTileMap.h"

// collision systems
#include "Game/CollisionLogic/CollisionSystem/CollisionDetectionSystem.h"

// skill
#include "Game/SkillSystem/Trigger/PlayerSkillTriggerSystem.h"
#include "Game/SkillSystem/System/SkillCastingSystem.h"
#include "Game/SkillSystem/MasterData/SkillSlot.h"
#include "Game/SkillSystem/System/UpdateSkillLifetimes.h"

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
	, mInputManager(nullptr)
	, mCollisionResults()
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

	delete mInputManager;
	mInputManager = nullptr;


	mWindow.Shutdown();

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
	
	if (!mWindow.Initialize(windowWidth, windowHeight, "Game"))
	{
		std::cerr << "[Game.cpp]: Failed to Initialize WindowManager" << std::endl;
		return false;
	}
	
	// initialize input system
	//InputManager::Initialize(WindowManager::GetWindow());

	mInputManager = new InputManager(mWindow.GetGLFWWindow());

	// WindowManager::CaptureMouse();

	glEnable(GL_DEPTH_TEST);


	// ShaderInit
	mShader = new Shader("shaders/basic.vertex.glsl", "shaders/basic.fragment.glsl");


	// Log
	std::cout << "[Game.cpp (Initialize)]: Application initialization completed successfully" << std::endl;


	// initialize input mapping
	InitializeInputMapping();

	// Initialize Skill database
	InitializeSkills();

	InitializeSkillMappings();

	loadData();

	return true;
}

void Game::RunLoop()
{
	while (!glfwWindowShouldClose(mWindow.GetGLFWWindow()) && mIsRunning)
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
	// Delta Time
	float currentFrame = static_cast<float>(glfwGetTime());
	mDeltaTime = currentFrame - mLastFrame;
	mLastFrame = currentFrame;
	// std::cout << "[Game.cpp(DeltaTime)]: deltaTime: " << mDeltaTime << "\n";

	// コリジョンコンテキスト: 1フレームごとに初期化
	mCollisionResults.Clear();

	// Input
	mInputManager->Update();
 
	const RawInputState& input = mInputManager->GetRawInput();;
	if (input.keyState.count(GLFW_KEY_ESCAPE) && input.keyState.at(GLFW_KEY_ESCAPE)) {
		mIsRunning = false;
	}

	InputRouterSystem(mEcs, mInputManager->GetRawInput(), mInputMapping);

	// 入力状態マップの更新
	//mInputMapping.update(mWindow.GetGLFWWindow(), mInputState);

	// characterの移動
	PlayerCharacterControlSystem::Update(mEcs, mInputManager->GetRawInput(), mRenderContext, mDeltaTime);
	//PlayerCharacterControlSystem::Update(mEcs, mInputState, mDeltaTime, mRenderContext);
	// PlayerCharacterControlSystem::Update(mEcs, mInputState, mDeltaTime);

	// skill system
	SkillSystem::Trigger::PlayerSkillTriggerSystem::TriggerSkillsFromInput(mEcs, mSkillInputMap);
	SkillSystem::Casting::SpawnSkillHitArea(mEcs, mSkillDatabase);
	SkillSystem::Lifetime::UpdateSkillLifetimes(mEcs, mDeltaTime, mSkillDatabase);
	// SkillSystem::Trigger::PlayerSkillTriggerSystem::Update(mEcs, mSkillInputMap);
	// SkillSystem::SkillCastingSystem(mEcs, mSkillDatabase, mRenderContext, mDeltaTime);

	// 2D (Logic)-> 3D (Drawing)
	SyncLogicToTransformSystem::Apply2DToTransform(mEcs, mDeltaTime);

	// カメラ
	CameraFollowSystem::Update(mEcs, mDeltaTime);
	// GameSystemInput::UpdateCamera(mEcs, mInputState, mDeltaTime);


	Game::updateContext();

	// Update Mouse Cursor Logic data 
	MouseCursorUpdateSystem::Update(mEcs, mInputManager->GetRawInput(), mRenderContext);
	//MouseCursorUpdateSystem::Update(mEcs, mInputState, mRenderContext);

	// 
	GameUtils::CollisionLogic::DetectionSystem::UpdateCollisionResultStorage(mEcs, mCollisionResults);
}

void Game::generateOutputs()
{
	glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// An algorithm is needed to set the shader for each object.
	// RenderSystem::RenderSystem(mEcs, *mShader, WindowManager::GetAspect());
	RenderSystem::RenderSystem(mEcs, *mShader, mWindow.GetAspect(), mRenderContext);


	// draw for debugging
	LogicDebugDrawSystem::Draw(mEcs, mRenderContext, mCollisionResults);


	//
	glfwSwapBuffers(mWindow.GetGLFWWindow());
}

void Game::loadData()
{
	Game::spawnAllActors();

	Game::RunInitializationPhase();

	std::cout << "[Game.cpp]: Data loading completed successfully." << std::endl;
}

void Game::unloadData()
{

}


void Game::spawnAllActors()
{
	//TriangleActor tri = TriangleActor(mEcs);

//Test3DModel test3d = Test3DModel(mEcs, mShader);

	PlayerCharacter player = PlayerCharacter(mEcs, mShader);

	FollowCameraActor followCam = FollowCameraActor(mEcs);

	MouseCursorActor mouseCursor = MouseCursorActor(mEcs);

	// TileMapActor tilemap = TileMapActor(mEcs);
	// TestRockActor testRock = TestRockActor(mEcs, mShader);


	TestBaseTerrainActor testTerrainMap = TestBaseTerrainActor(mEcs, mShader);

	TestRockActor testRock = TestRockActor(mEcs, mShader);

	// CameraActor camActor = CameraActor(mEcs);

	//Collider a, c;
	//c.type = ColliderType::Box2D;
	//a.type = ColliderType::Box2D;
	//c.box2D = Box2D{ glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	//a.box2D = Box2D { glm::vec2(3.0f, 0.0f) , glm::vec2(1.0f,1.0f) };

	//bool hit = CollisionUtils::intersectBox2D(a.box2D, c.box2D);

	//std::cout << "Result: " << std::boolalpha << hit << std::endl;
}

void Game::RunInitializationPhase()
{
	//for (Entity e : mEcs.view<TileMapComponent>())
	//{
	//	auto& tileMapComp = mEcs.get<TileMapComponent>(e);
	//	GameInit::TileMapFromMesh::ApplyObstacleCollidersToTileMap(mEcs, tileMapComp);
	//}

	//for (Entity e : mEcs.view<FollowCameraComponent, CameraComponent, TransformComponent>())
	//{
	//	auto& followCamComp = mEcs.get<FollowCameraComponent>(e);
	//	InitSystem<FollowCameraComponent>::Init(followCamComp, mEcs, e);
	//}

	ApplyAllDeferredInitializations<
		TileMapComponent,
		FollowCameraComponent
	// コンテキスト情報を渡す．
	>(mEcs, mWindow);
}

void Game::updateContext()
{
	// update RenderContext:: viewport 
	mRenderContext.viewport = glm::vec4(0, 0, mWindow.GetWidth(), mWindow.GetHeight());
}

void Game::InitializeInputMapping()
{
	mInputMapping.bindKey(GLFW_KEY_W, InputAction::MoveForward);
	mInputMapping.bindKey(GLFW_KEY_S, InputAction::MoveBackward);
	mInputMapping.bindKey(GLFW_KEY_D, InputAction::MoveRight);
	mInputMapping.bindKey(GLFW_KEY_A, InputAction::MoveLeft);
	mInputMapping.bindKey(GLFW_MOUSE_BUTTON_1, InputAction::CastSkill1);
	mInputMapping.bindKey(GLFW_MOUSE_BUTTON_2, InputAction::CastSkill2);
}

void Game::InitializeSkills()
{
	SkillDefinition slash;
	slash.id = 1;
	slash.name = "Basic Slash";
	slash.shape = Attack2DShape{ Circle2DAttack{glm::vec2(0.0f), 5.0f} };
	slash.duration = 2.0f;
	mSkillDatabase.AddSkill(slash);

	SkillDefinition slash2;
	slash2.id = 2;
	slash2.name = "Power Slash";
	slash2.shape = Attack2DShape{ Sector2DAttack{glm::vec2(0.0f), glm::vec2(1.0f), 1.0f, 10.0f} };
	slash2.duration = 2.0f;
	mSkillDatabase.AddSkill(slash2);
}

void  Game::InitializeSkillMappings()
{
	mSkillInputMap.bind(InputAction::CastSkill1, SkillSystem::SkillSlot::Primary);// スキルID 1
	mSkillInputMap.bind(InputAction::CastSkill2, SkillSystem::SkillSlot::Secondary);// スキルID 2
}