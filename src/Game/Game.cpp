#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std system
#include <iostream>

// ======================= Engine =======================

//Entity
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Private/Core/World.hpp"
#include "Engine/WorldSystem/Private/Core/WorldCtx.hpp"

#include "Engine/ECS/GlobalSystem/GlobalCleanupSystem.h"

//Components
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

// Init Components (Must be included for specialization)
#include "Engine/ECS/Public/ECSApi.hpp"
#include "Engine/ECS/Public/InitSpecialization/FollowCamInit.hpp"

// #include "Engine/ECS/Meta/InitComponent/InitDispatcher.h"
#include "Engine/ECS/Meta/InitComponent/TileMapInit.h"
// #include "Engine/ECS/Meta/InitComponent/FollowCameraInit.h"


// Window
#include "Engine/Window/Public/Window.h"

// input
#include "Engine/InputManager/Public/Types.hpp"

// physics
#include "Engine/Physics/Private/Logic2D/DetectionFunctions.h"
#include "Engine/ECS/Component/Logic2D/ColliderType.h"

// Graphic
#include "Engine/Graphics/Private/Renderer/RenderSystem.h"
#include "Engine/Graphics/Private/Model/AssimpImporter.h"

#include "Engine/Graphics/Public/GraphicsApi.hpp"

// Time
#include "Engine/Time/Private/WorldClock.hpp"

// Debug
#include "Engine/Debug/Private/DebugSystems/LogicDebugDrawSystem.h"
#include "Engine/Config/CanonicalDefaults.h"

// ======================= Game =======================

//Game/ Feature
#include "Game/00_Feature/Private/Character/Movement/MovementFeature.h"
//Game/ Layer
#include "Game/01_Layer/Private/AllLayerFeatureSystem.hpp"
#include "Game/01_Layer/Public/LayerFeature.hpp"

//Game/Actor
#include "Game/Actor/CameraActor.h"
#include "Game/Actor/PlayerCharacterActor.h"
#include "Game/Actor/FollowCameraActor.h"
#include "Game/Actor/MouseCursorActor.h"
#include "Game/Actor/Map/TestBaseTerrainActor.h"
#include "Game/Actor/TestObject.h"
//
#include "Game/Actor/Public/TestPlayerActor.hpp"
#include "Game/Actor/Public/TestCamActor.hpp"
#include "Game/Actor/Public/TestmapActor.hpp"

// Game/Actor/Map
#include "Game/Actor/Map/TileMapActor.h"
#include "Game/Actor/Map/Obstacle/TestRockActor.h"

// Game/Camera
#include "Game/Camera/Private/CameraFollowSystem.h"
#include "Game/Camera/Public/CameraApi.hpp"

// Game/Input

// Game/Input/Intent


// Game Init
#include "Game/Init/Private/InitTileMap/InitTileMap.h"
#include "Game/Init/Private/InitComponent/InputBindingInit.h"

// collision systems


// skill
#include "Game/Combat/Private/Skill/System/Trigger/PlayerSkillTriggerSystem.h"
#include "Game/Combat/Private/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Private/Skill/MasterData/SkillSlot.h"
#include "Game/Combat/Private/Skill/System/UpdateSkillLifetimes.h"
#include "Game/Combat/Private/Skill/System/UpdateSkillPhase.h"

// skill trajectory
#include "Game/Combat/Private/Skill/Component/SkillTrajectoryComponent.h"
#include "Game/Combat/Private/Skill/MasterData/SkillTrajectoryData.h"
#include "Game/Combat/Private/Skill/System/SkillTrajectorySystem.h"

// Game/Sync
#include "Engine/Sync/Private/LogicToTransformSystem.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

// Test
//#include "Test/TriangleActor.h"
//#include "Test/Test3DModel.h"

// コンストラクタ
GameApp::GameApp::GameApp()
	: isRunning_(true)
	, windowWidth_(1280)
	, windowHeight_(720)
{
	world_ = std::make_unique<Engine::WorldSystem::Core::World>();
}

GameApp::GameApp::~GameApp() = default;

// 終了処理
void GameApp::GameApp::Shutdown()
{
	unloadData();

	// Destroy all components
	// mCtx = {};

	//mECS.Clear();


	//mShader.reset();
	input_.reset();

	shader_.reset();


	if(window_)window_->Shutdown();
	window_.reset();

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
	

	window_ = std::make_unique<Engine::Window::Window>();
	if (!window_->Initialize(windowWidth_, windowHeight_, "Game"))
	{
		std::cerr << "[Game.cpp]: Failed to Initialize WindowManager" << std::endl;
		return false;
	}
	
	// initialize input system
	input_ = std::make_unique<Engine::Input::InputManager>(window_->GetGLFWWindow());

	// shader
	shader_ = std::make_unique<Engine::Graphics::Shader>
		(
			"shaders/basic.vertex.glsl", "shaders/basic.fragment.glsl"
		);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);// 


	// 遅らせ初期化
	// Game::Layer::InitializeLayerFeature::DelayedInitialzation(mECS);

	Engine::WorldSystem::Core::WorldCtx ctx{ *world_ };
	Game::Layer::InitializeLayerFeature::DelayedInitialization(ctx);

	// loadData();
	// ctxバージョン
	loadData(ctx);

	return true;
}

void GameApp::GameApp::RunLoop()
{


	while (!glfwWindowShouldClose(window_->GetGLFWWindow()) && isRunning_)
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		// mDeltaTime = currentFrame - mLastFrame;
		float deltaTime = currentFrame - lastFrame_;
		lastFrame_ = currentFrame;
		// std::cout << "[Game.cpp(DeltaTime)]: deltaTime: " << deltaTime << "\n";

		Engine::WorldSystem::Core::WorldCtx ctx{ *world_ };

		//updateGameLogics(deltaTime);
		// updateGameLogics();
		//generateOutputs();

		glfwPollEvents();

		// ctxバージョン
		Update(ctx, deltaTime);
	}

	// Update();
}

void GameApp::GameApp::updateGameLogics(float deltaTime)

{	// Delta Time

	//float currentFrame = static_cast<float>(glfwGetTime());
	//// mDeltaTime = currentFrame - mLastFrame;
	//float deltaTime = currentFrame - mLastFrame;
	//mLastFrame = currentFrame;
	// std::cout << "[Game.cpp(DeltaTime)]: deltaTime: " << deltaTime << "\n";

	// debug用


	// delete PendingDestroyComponent
	//Engine::ECS::GrobalSystem::RunCleanup(mECS);



	// Input
	//mInput->Update();
 
	//const Engine::Input::RawInputState& input = mInput->GetRawInput();
	//if (input.keyState.count(GLFW_KEY_ESCAPE) && input.keyState.at(GLFW_KEY_ESCAPE)) {
	//	mIsRunning = false;
	//}

	// ECSのグローバルリソースからMappingを取得して，InputActionComponentを変更するように修正する
	// 修正済みのため削除予定
	// Game::Input::InputRouterSystem(mECS, mInputManager->GetRawInput(), mInputMapping);
	// InputRouter
	//Game::Input::InputRouterSystem(mECS, mInput->GetRawInput());
	//Game::Input::Analog::RouteAnalogInput(mECS, mInput->GetRawInput(), mRenderCtx);

	// 2D (Logic)-> 3D (Drawing)
	//Engine::Sync::LogicToTransformSystem::Apply2DToTransform(mECS, deltaTime);

	// ctx 終わった
	// カメラ
	// Game::Camera::Update(mECS, deltaTime);
	// GameSystemInput::UpdateCamera(mEcs, mInputState, mDeltaTime);
	

	//GameApp::updateContext();

	//Game::Collision::System::UpdateCollisionResultStorage(mECS, mCollisionResults);

	//// Update from the top layer	
	//Game::Layer::IntentLayerFeature::Update(mECS);

	// Resolver Layer
	//Game::Layer::ResolverLayerFeature::Update(mECS);

	// state layer
	//Game::Layer::StateLayerFeature::Update(mECS, deltaTime);

	// Logic Layer
	//Game::Layer::LogicLayerFeature::Update(mECS, deltaTime);

	// Anim Layer
	// locomotion
	//Game::Layer::LocomotionResolverLayerFeature::Update(mECS);
	// skill
	//Game::Layer::SkillAnimationResolverLayerFeature::Update(mECS);
	// cc
	//Game::Layer::CCAnimationResolverLayerFeature::Update(mECS);

	// ctx 終わった
	// Anim Layer へ移動
	// Engine::Graphics::Animation::System::AnimationSystem(mECS);
}

void GameApp::GameApp::generateOutputs()
{
	//glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// ctx おわった
	// An algorithm is needed to set the shader for each object.
	// RenderSystem::RenderSystem(mEcs, *mShader, WindowManager::GetAspect());
	// draw Layer へ移動
	// Engine::Graphics::Render::RenderSystem(mECS, *mShader, mWindow->GetAspect(), mRenderCtx);

	// ctx まだ
	// Draw Layerへ移動
	// Engine::Debug::Drawing::Logic2D::Draw(mECS, mRenderCtx, mCollisionResults);
	

	//
	//glfwSwapBuffers(window_->GetGLFWWindow());
}

void GameApp::GameApp::loadData()
{
	//GameApp::spawnAllActors();

	//GameApp::RunInitializationPhase();

	std::cout << "[Game.cpp]: Data loading completed successfully." << std::endl;
}

void GameApp::GameApp::unloadData()
{

}


void GameApp::GameApp::spawnAllActors()
{
	//TriangleActor tri = TriangleActor(mEcs);

	//Test3DModel test3d = Test3DModel(mEcs, mShader);

	//Game::Actor::Player::PlayerCharacter player = Game::Actor::Player::PlayerCharacter(mECS, mShader.get());

	//Game::Actor::Camera::FollowCameraActor followCam = Game::Actor::Camera::FollowCameraActor(mECS);

	//Game::Actor::AnalogInput::MouseCursorActor mouseCursor = Game::Actor::AnalogInput::MouseCursorActor(mECS);

	// TileMapActor tilemap = TileMapActor(mEcs);
	// TestRockActor testRock = TestRockActor(mEcs, mShader);


	//Game::Actor::Map::TestBaseTerrainActor testTerrainMap = Game::Actor::Map::TestBaseTerrainActor(mECS, mShader.get());

	//Game::Actor::Map::TestRockActor testRock = Game::Actor::Map::TestRockActor(mECS, mShader.get());

	//Game::Actor::TestObject testObj = Game::Actor::TestObject(mECS, mShader.get());

	// CameraActor camActor = CameraActor(mEcs);

}


void GameApp::GameApp::RunInitializationPhase()
{
	// Engine::ECS::Meta::Init::ApplyAllDeferredInitializations<
	// 	Engine::ECS::Component::Logic2D::TileMapComponent,
	// 	Engine::ECS::Component::Camera::FollowCameraComponent
	// // コンテキスト情報を渡す．
	// >(mECS, *mWindow);

	// Game::Init::Input::InputBindingInitializationSystem(mECS);
}



void GameApp::GameApp::updateContext()
{
	// update RenderContext:: viewport 
	renderCtx_.viewport = glm::vec4(0, 0, window_->GetWidth(), window_->GetHeight());
}


// -- world --
// 
void GameApp::GameApp::RunInitializationPhase(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	Engine::ECS::Core::Init::ApplyAllDefferedInit<
		Engine::Component::FollowCameraComponent
	>(ctx, *window_);

	// input
	Game::Init::Input::InputBindingInitializationSystem(ctx);
}

void GameApp::GameApp::spawnAllActors(Engine::WorldSystem::Core::WorldCtx& ctx)
{

	Game::Actor::TestPlayerActor testActor = Game::Actor::TestPlayerActor(ctx, shader_.get());
	//Game::Actor::TestFixedCamActor fixedCam = Game::Actor::TestFixedCamActor(ctx);
	Game::Actor::TestFollowCamActor followCam = Game::Actor::TestFollowCamActor(ctx);
	Game::Actor::TestPlayerCursorActor cursor = Game::Actor::TestPlayerCursorActor(ctx);
	Game::Actor::TestTerrainMesh terrain = Game::Actor::TestTerrainMesh(ctx, shader_.get());
}


void GameApp::GameApp::updateGameLogics(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	GameApp::updateContext();
	input_->Update();
	const auto& input = input_->GetRawInput();
	if (input.keyState.count(GLFW_KEY_ESCAPE) && input.keyState.at(GLFW_KEY_ESCAPE))
	{
		isRunning_ = false;
	}

	Game::Layer::InputLayer::Update(ctx, input_->GetRawInput(), renderCtx_);

	Game::Layer::IntentLayerFeature::Update(ctx);
	Game::Layer::StateLayerFeature::Update(ctx);


	Game::Layer::LogicLayerFeature::Update(ctx);
}

void GameApp::GameApp::generateOutputs(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	// Game::Camera::Update(ctx);

	glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Engine::Graphics::UpdateRendererAll(ctx, *shader_, window_->GetAspect(), renderCtx_);
	Game::Layer::LocomotionAnimLayerFeature::Update(ctx);
	Game::Layer::DrawLayerFeature::Update(ctx, *shader_, *window_, renderCtx_);


	glfwSwapBuffers(window_->GetGLFWWindow());
}

void GameApp::GameApp::Update(Engine::WorldSystem::Core::WorldCtx& ctx, float realDt)
{

	Engine::Time::WorldClockSystem::BeginFrame(ctx, realDt);

	while (Engine::Time::WorldClockSystem::PopFixedStep(ctx))
	{
		updateGameLogics(ctx);
	}

	generateOutputs(ctx);

	// 
	Engine::Time::WorldClockSystem::EndFrame(ctx);
}


void GameApp::GameApp::loadData(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	GameApp::spawnAllActors(ctx);

	GameApp::RunInitializationPhase(ctx);
}


// FSM関係以外の現状復帰を目指す
// 後は、Collision だけ

// Input までOK
// 次は、移動による座標更新
// カメラ追尾 OK
// 
//

// FSM: Skill / CC
// FSM: Interference
// 

// Skill Input -> Intent -> FSM -> ...系
// 初期化処理実装必要 (SkillLoadoutDTO/SkillCatalogの登録)
// 




// TODO: 
// Collision システムのデータ駆動化 / ライブラリ構造化
//