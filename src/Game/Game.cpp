#include "Game.h"

// std system
#include <iostream>

// ======================= Engine =======================

//Entity
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"


#include "Engine/ECS/GlobalSystem/GlobalCleanupSystem.h"

//Components
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

// Init Components (Must be included for specialization)
#include "Engine/ECS/Meta/InitComponent/InitDispatcher.h"
#include "Engine/ECS/Meta/InitComponent/TileMapInit.h"
#include "Engine/ECS/Meta/InitComponent/FollowCameraInit.h"

// Window

// physics
#include "Engine/Physics/Logic2D/DetectionFunctions.h"
#include "Engine/ECS/Component/Logic2D/ColliderType.h"

// Graphic
#include "Engine/Graphics/Renderer/RenderSystem.h"
#include "Engine/Graphics/Model/AssimpImporter.h"
#include "Engine/Graphics/Animation/AnimationSystem.hpp"

// Debug
#include "Engine/Debug/DebugSystems/LogicDebugDrawSystem.h"
#include "Engine/Config/CanonicalDefaults.h"

// ======================= Game =======================

//Game/ Feature
#include "Game/00_Feature/Character/Movement/MovementFeature.h"
//Game/ Layer
#include "Game/01_Layer/InitializeLayerFeature.h"
#include "Game/01_Layer/InputLayerFeature.h"
#include "Game/01_Layer/IntentLayerFeature.h"
#include "Game/01_Layer/ResolverLayerFeature.hpp"
#include "Game/01_Layer/LogicLayerFeature.h"
#include "Game/01_Layer/StateLayerFeature.hpp"
#include "Game/01_Layer/LocomotionResolverLayerFeature.hpp"

//Game/Actor
#include "Game/Actor/CameraActor.h"
#include "Game/Actor/PlayerCharacterActor.h"
#include "Game/Actor/FollowCameraActor.h"
#include "Game/Actor/MouseCursorActor.h"
#include "Game/Actor/Map/TestBaseTerrainActor.h"
#include "Game/Actor/TestObject.h"

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
#include "Game/Input/AnalogInput/AnalogInputRoutingSystem.h"

// Game/Input/Intent
#include "Game/Input/Intent/PlayerMovementIntentMapper.h"

// Game Init
#include "Game/Init/InitTileMap/InitTileMap.h"
#include "Game/Init/InitComponent/InputBindingInit.h"

// collision systems
#include "Game/Collision/System/CollisionDetectionSystem.h"

// skill
#include "Game/Combat/Skill/System/Trigger/PlayerSkillTriggerSystem.h"
#include "Game/Combat/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Skill/MasterData/SkillSlot.h"
#include "Game/Combat/Skill/System/UpdateSkillLifetimes.h"
#include "Game/Combat/Skill/System/UpdateSkillPhase.h"

// skill trajectory
#include "Game/Combat/Skill/Component/SkillTrajectoryComponent.h"
#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"
#include "Game/Combat/Skill/System/SkillTrajectorySystem.h"

// Game/Sync
#include "Engine/Sync/LogicToTransformSystem.h"



// Test
//#include "Test/TriangleActor.h"
//#include "Test/Test3DModel.h"

// コンストラクタ
GameApp::GameApp::GameApp()
	: mIsRunning(true)
	, windowWidth(1280)
	, windowHeight(720)
{

}

// 終了処理
void GameApp::GameApp::Shutdown()
{
	unloadData();

	// Destroy all components
	mCtx = {};

	mECS.Clear();


	mShader.reset();
	mInput.reset();


	if(mWindow)mWindow->Shutdown();
	mWindow.reset();

	std::cout << "\n[Game.cpp (Shutdown)]: The application shut down successfully." << std::endl;
}

// 初期化
bool GameApp::GameApp::Initialize()
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
	
	mWindow = std::make_unique<Engine::Window::Window>();
	if (!mWindow->Initialize(windowWidth, windowHeight, "Game"))
	{
		std::cerr << "[Game.cpp]: Failed to Initialize WindowManager" << std::endl;
		return false;
	}
	
	// initialize input system
	//InputManager::Initialize(WindowManager::GetWindow());

	mInput = std::make_unique<Engine::Input::InputManager>(mWindow->GetGLFWWindow());
	mShader = std::make_unique<Engine::Graphics::Render::Shader>
		(
			"shaders/basic.vertex.glsl", "shaders/basic.fragment.glsl"
		);
	// WindowManager::CaptureMouse();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);// sRGB

	// ShaderInit
	// mShader = new eNsGfxRender::Shader("shaders/basic.vertex.glsl", "shaders/basic.fragment.glsl");

	// Log
	// std::cout << "[Game.cpp (Initialize)]: Application initialization completed successfully" << std::endl;

	mCtx.ecs = &mECS;
	mCtx.renderCtx = &mRenderCtx;
	mCtx.window = mWindow.get();
	mCtx.input = mInput.get();
	mCtx.shader = mShader.get();



	// Initialize Skill database
	// InitializeSkills();

	// 遅らせ初期化
	gNsLayer::InitializeLayerFeature::DelayedInitialzation(mECS);

	loadData();

	return true;
}

void GameApp::GameApp::RunLoop()
{
	while (!glfwWindowShouldClose(mWindow->GetGLFWWindow()) && mIsRunning)
	{
		updateGameLogics();
		generateOutputs();
		glfwPollEvents();
	}
}

void GameApp::GameApp::updateGameLogics()

{	// Delta Time
	float currentFrame = static_cast<float>(glfwGetTime());
	// mDeltaTime = currentFrame - mLastFrame;
	float deltaTime = currentFrame - mLastFrame;
	mLastFrame = currentFrame;
	// std::cout << "[Game.cpp(DeltaTime)]: deltaTime: " << deltaTime << "\n";

	// delete PendingDestroyComponent
	eNsECS::GrobalSystem::RunCleanup(mECS);



	// Input
	mInput->Update();
 
	const eNsInput::RawInputState& input = mInput->GetRawInput();
	if (input.keyState.count(GLFW_KEY_ESCAPE) && input.keyState.at(GLFW_KEY_ESCAPE)) {
		mIsRunning = false;
	}

	// ECSのグローバルリソースからMappingを取得して，InputActionComponentを変更するように修正する
	// 修正済みのため削除予定
	// gNsInput::InputRouterSystem(mECS, mInputManager->GetRawInput(), mInputMapping);
	// InputRouter
	gNsInput::InputRouterSystem(mECS, mInput->GetRawInput());
	gNsInput::Analog::RouteAnalogInput(mECS, mInput->GetRawInput(), mRenderCtx);

	// 2D (Logic)-> 3D (Drawing)
	eNsSyncL2T::Apply2DToTransform(mECS, deltaTime);

	// カメラ
	gNsCam::Update(mECS, deltaTime);
	// GameSystemInput::UpdateCamera(mEcs, mInputState, mDeltaTime);

	GameApp::updateContext();

	Game::Collision::System::UpdateCollisionResultStorage(mECS, mCollisionResults);

	// Update from the top layer	
	gNsLayer::IntentLayerFeature::Update(mECS);

	// Resolver Layer
	gNsLayer::ResolverLayerFeature::Update(mECS);

	// state layer
	gNsLayer::StateLayerFeature::Update(mECS, deltaTime);

	// Logic Layer
	gNsLayer::LogicLayerFeature::Update(mECS, deltaTime);

	// Anim Layer
	// locomotion
	Game::Layer::LocomotionResolverLayerFeature::Update(mECS);
	// skill
	Game::Layer::SkillAnimationResolverLayerFeature::Update(mECS);

	// Anim Layer へ移動
	Engine::Graphics::Animation::System::AnimationSystem(mECS);
}

void GameApp::GameApp::generateOutputs()
{
	glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// An algorithm is needed to set the shader for each object.
	// RenderSystem::RenderSystem(mEcs, *mShader, WindowManager::GetAspect());
	// draw Layer へ移動
	eNsGfxRender::RenderSystem(mECS, *mShader, mWindow->GetAspect(), mRenderCtx);

	// Draw Layerへ移動
	Engine::Debug::Drawing::Logic2D::Draw(mECS, mRenderCtx, mCollisionResults);

	//
	glfwSwapBuffers(mWindow->GetGLFWWindow());
}

void GameApp::GameApp::loadData()
{
	GameApp::spawnAllActors();

	GameApp::RunInitializationPhase();

	std::cout << "[Game.cpp]: Data loading completed successfully." << std::endl;
}

void GameApp::GameApp::unloadData()
{

}


void GameApp::GameApp::spawnAllActors()
{
	//TriangleActor tri = TriangleActor(mEcs);

//Test3DModel test3d = Test3DModel(mEcs, mShader);

	gNsActorPlayer::PlayerCharacter player = gNsActorPlayer::PlayerCharacter(mECS, mShader.get());

	gNsActorCam::FollowCameraActor followCam = gNsActorCam::FollowCameraActor(mECS);

	gNsActorAnalogInput::MouseCursorActor mouseCursor = gNsActorAnalogInput::MouseCursorActor(mECS);

	// TileMapActor tilemap = TileMapActor(mEcs);
	// TestRockActor testRock = TestRockActor(mEcs, mShader);


	gNsActorMap::TestBaseTerrainActor testTerrainMap = gNsActorMap::TestBaseTerrainActor(mECS, mShader.get());

	gNsActorMap::TestRockActor testRock = gNsActorMap::TestRockActor(mECS, mShader.get());

	gNsActor::TestObject testObj = gNsActor::TestObject(mECS, mShader.get());

	// CameraActor camActor = CameraActor(mEcs);


}

void GameApp::GameApp::RunInitializationPhase()
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

	eNsECSInitComp::ApplyAllDeferredInitializations<
		eNsLogic2DComp::TileMapComponent,
		eNsCamComp::FollowCameraComponent
	// コンテキスト情報を渡す．
	>(mECS, *mWindow);

	gNsInit::Input::InputBindingInitializationSystem(mECS);
}

void GameApp::GameApp::updateContext()
{
	// update RenderContext:: viewport 
	mRenderCtx.viewport = glm::vec4(0, 0, mWindow->GetWidth(), mWindow->GetHeight());
}

