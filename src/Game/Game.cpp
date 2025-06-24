#include "Game.h"

// std system
#include <iostream>

// ======================= Engine =======================

//Entity
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Meta/InitComponent/FollowCameraInit.h"

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
#include "Game/01_Layer/LogicLayerFeature.h"
#include "Game/01_Layer/StateLayerFeature.hpp"

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
	, mShader(nullptr)
	, mRenderContext()
	, mInputManager(nullptr)
	, mCollisionResults()
	, windowWidth(1280)
	, windowHeight(720)
{

}

// 終了処理
void GameApp::GameApp::Shutdown()
{
	unloadData();

	// Destroy all components
	mECS.Clear();

	delete mShader;
	mShader = nullptr;

	delete mInputManager;
	mInputManager = nullptr;


	mWindow.Shutdown();

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
	
	if (!mWindow.Initialize(windowWidth, windowHeight, "Game"))
	{
		std::cerr << "[Game.cpp]: Failed to Initialize WindowManager" << std::endl;
		return false;
	}
	
	// initialize input system
	//InputManager::Initialize(WindowManager::GetWindow());

	mInputManager = new eNsInput::InputManager(mWindow.GetGLFWWindow());

	// WindowManager::CaptureMouse();

	glEnable(GL_DEPTH_TEST);


	// ShaderInit
	mShader = new eNsGfxRender::Shader("shaders/basic.vertex.glsl", "shaders/basic.fragment.glsl");


	// Log
	std::cout << "[Game.cpp (Initialize)]: Application initialization completed successfully" << std::endl;


	// initialize input mapping
	InitializeInputMapping();

	// Initialize Skill database
	// InitializeSkills();

	// 遅らせ初期化
	gNsLayer::InitializeLayerFeature::DelayedInitialzation(mECS);


	InitializeSkillMappings();


	loadData();

	return true;
}

void GameApp::GameApp::RunLoop()
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

void GameApp::GameApp::updateGameLogics()

{	// Delta Time
	float currentFrame = static_cast<float>(glfwGetTime());
	// mDeltaTime = currentFrame - mLastFrame;
	float deltaTime = currentFrame - mLastFrame;
	mLastFrame = currentFrame;
	// std::cout << "[Game.cpp(DeltaTime)]: deltaTime: " << mDeltaTime << "\n";

	// delete PendingDestroyComponent
	eNsECS::GrobalSystem::RunCleanup(mECS);

	// ECSのグローバルリソースへ以降予定(移行後削除予定)
	// コリジョンコンテキスト: 1フレームごとに初期化
	mCollisionResults.Clear();

	// Input
	mInputManager->Update();
 
	const eNsInput::RawInputState& input = mInputManager->GetRawInput();;
	if (input.keyState.count(GLFW_KEY_ESCAPE) && input.keyState.at(GLFW_KEY_ESCAPE)) {
		mIsRunning = false;
	}

	// ECSのグローバルリソースからMappingを取得して，InputActionComponentを変更するように修正する
	// 修正済みのため削除予定
	// gNsInput::InputRouterSystem(mECS, mInputManager->GetRawInput(), mInputMapping);
	// InputRouter
	gNsInput::InputRouterSystem(mECS, mInputManager->GetRawInput());
	gNsInput::Analog::RouteAnalogInput(mECS, mInputManager->GetRawInput(), mRenderContext);
	// 入力状態マップの更新
	//mInputMapping.update(mWindow.GetGLFWWindow(), mInputState);

	// ====INTENT====
	// gNsInputIntent::IntentMappingSystem::UpdatePlayerMovementIntent(mECS);

	
	// characterの移動 (削除予定(Intentレイヤー導入のため))
	// gNsInput::Player::Update(mECS, mInputManager->GetRawInput(), mRenderContext, mDeltaTime);
	//PlayerCharacterControlSystem::Update(mEcs, mInputState, mDeltaTime, mRenderContext);
	// PlayerCharacterControlSystem::Update(mEcs, mInputState, mDeltaTime);

	// skill system (削除予定(レイヤー構造導入のため))
	// gNsSkillTrigger::PlayerSkillTriggerSystem::TriggerSkillsFromInput(mECS, mSkillInputMap);
	// SkillSystem::Casting::SpawnSkillHitArea(mEcs, mSkillDatabase);
	// gNsSkillSystem::UpdateSkillPhase(mECS, mDeltaTime, mSkillDatabase);
	// gNsSkillSystem::SkillTrajectorySystem::Update(mECS, mDeltaTime);
	// SkillSystem::Lifetime::UpdateSkillLifetimes(mEcs, mDeltaTime, mSkillDatabase);
	// SkillSystem::Trigger::PlayerSkillTriggerSystem::Update(mEcs, mSkillInputMap);
	// SkillSystem::SkillCastingSystem(mEcs, mSkillDatabase, mRenderContext, mDeltaTime);

	// 2D (Logic)-> 3D (Drawing)
	eNsSyncL2T::Apply2DToTransform(mECS, deltaTime);

	// カメラ
	gNsCam::Update(mECS, deltaTime);
	// GameSystemInput::UpdateCamera(mEcs, mInputState, mDeltaTime);


	GameApp::updateContext();

	// Update Mouse Cursor Logic data (削除予定(Intent レイヤー導入のため))
	// gNsInput::Analog::Update(mECS, mInputManager->GetRawInput(), mRenderContext);
	//MouseCursorUpdateSystem::Update(mEcs, mInputState, mRenderContext);



	// ECSのグローバルリソースへ以降予定(移行後削除予定)
	gNsCollSystem::UpdateCollisionResultStorage(mECS, mCollisionResults);

	// Update from the top layer	
	gNsLayer::IntentLayerFeature::Update(mECS);

	gNsLayer::LogicLayerFeature::Update(mECS, deltaTime);

	gNsLayer::StateLayerFeature::Update(mECS, deltaTime);
}

void GameApp::GameApp::generateOutputs()
{
	glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// An algorithm is needed to set the shader for each object.
	// RenderSystem::RenderSystem(mEcs, *mShader, WindowManager::GetAspect());
	eNsGfxRender::RenderSystem(mECS, *mShader, mWindow.GetAspect(), mRenderContext);


	// draw for debugging
	// (ECSグローバルリソース導入後変更必要)
	// (ECSグローバルリソース導入後変更必要)
	// (ECSグローバルリソース導入後変更必要)
	eNsDebugDraw::Logic2D::Draw(mECS, mRenderContext, mCollisionResults);


	//
	glfwSwapBuffers(mWindow.GetGLFWWindow());
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

	gNsActorPlayer::PlayerCharacter player = gNsActorPlayer::PlayerCharacter(mECS, mShader);

	gNsActorCam::FollowCameraActor followCam = gNsActorCam::FollowCameraActor(mECS);

	gNsActorAnalogInput::MouseCursorActor mouseCursor = gNsActorAnalogInput::MouseCursorActor(mECS);

	// TileMapActor tilemap = TileMapActor(mEcs);
	// TestRockActor testRock = TestRockActor(mEcs, mShader);


	gNsActorMap::TestBaseTerrainActor testTerrainMap = gNsActorMap::TestBaseTerrainActor(mECS, mShader);

	gNsActorMap::TestRockActor testRock = gNsActorMap::TestRockActor(mECS, mShader);

	gNsActor::TestObject testObj = gNsActor::TestObject(mECS, mShader);

	// CameraActor camActor = CameraActor(mEcs);

	//Collider a, c;
	//c.type = ColliderType::Box2D;
	//a.type = ColliderType::Box2D;
	//c.box2D = Box2D{ glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	//a.box2D = Box2D { glm::vec2(3.0f, 0.0f) , glm::vec2(1.0f,1.0f) };

	//bool hit = CollisionUtils::intersectBox2D(a.box2D, c.box2D);

	//std::cout << "Result: " << std::boolalpha << hit << std::endl;
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
	>(mECS, mWindow);

	gNsInit::Input::InputBindingInitializationSystem(mECS);
}

void GameApp::GameApp::updateContext()
{
	// update RenderContext:: viewport 
	mRenderContext.viewport = glm::vec4(0, 0, mWindow.GetWidth(), mWindow.GetHeight());
}

// 別処理か別ファイルへ分離予定(JSONもしくはCSVなどで与えるようにするべき)
// ゲーム中に設定で変更できるようにするべき
void GameApp::GameApp::InitializeInputMapping()
{
	//mInputMapping.bindKey(GLFW_KEY_W, gNsInput::InputAction::MoveForward);
	//mInputMapping.bindKey(GLFW_KEY_S, gNsInput::InputAction::MoveBackward);
	//mInputMapping.bindKey(GLFW_KEY_D, gNsInput::InputAction::MoveRight);
	//mInputMapping.bindKey(GLFW_KEY_A, gNsInput::InputAction::MoveLeft);
	//mInputMapping.bindKey(GLFW_MOUSE_BUTTON_1, gNsInput::InputAction::CastSkill1);
	//mInputMapping.bindKey(GLFW_MOUSE_BUTTON_2, gNsInput::InputAction::CastSkill2);
	//mInputMapping.bindKey(GLFW_KEY_1, gNsInput::InputAction::CastSkill3);

}

// 別処理か別ファイルへ分離予定(スキル定義はJSONもしくはCSVなどで与えるようにするべき)
// Combat/Skill/System/InitializeSkills.h へ責務以降 => Featureをインターフェースとして，InitializeLayerFeatureで呼び出す構造へ
// 削除予定( 別ファイルへ分離したため )
void GameApp::GameApp::InitializeSkills()
{
	//gNsSkillData::SkillDefinition slash;
	//slash.id = 1;
	//slash.name = "Basic Slash";
	//slash.shape = gNsSkillComp::Attack2DShape{ gNsSkillComp::Circle2DAttack{CanonicalDefaults::kLocalCenterXZ, 5.0f} };
	//slash.duration = 1.0f;
	//slash.trajectoryType = gNsSkillData::TrajectoryType::LinearForward;
	//slash.trajectoryParams = gNsSkillData::SkillTrajectory::LinearTrajectoryParams
	//{
	//	.speed = 20.0f
	//};
	//mSkillDatabase.AddSkill(slash);

	//gNsSkillData::SkillDefinition slash2;
	//slash2.id = 2;
	//slash2.name = "Power Slash";
	//slash2.shape = gNsSkillComp::Attack2DShape{ gNsSkillComp::Sector2DAttack{CanonicalDefaults::kLocalCenterXZ, CanonicalDefaults::kLocalForwardXZ, 1.0f, 10.0f} };// -Z方向が前方
	//slash2.duration = 1.0f;

	//mSkillDatabase.AddSkill(slash2);

	//gNsSkillData::SkillDefinition blade;
	//blade.id = 3;
	//blade.name = "Blade";
	//blade.shape = gNsSkillComp::Attack2DShape{ gNsSkillComp::Rectangle2DAttack{glm::vec2(0.0f, 5.0f), CanonicalDefaults::kLocalForwardXZ, 1.0f, 10.0f}};
	//blade.duration = 1.0f;
	//// スキル奇跡の抽象定義の選択
	//blade.trajectoryType = gNsSkillData::TrajectoryType::RotateAroundSelf;
	//blade.trajectoryParams = gNsSkillData::SkillTrajectory::RotateTrajectoryParams
	//{// 関数定義
	//	.startAngle = 60.0f,
	//	.endAngle = -60.0f
	//};
	//mSkillDatabase.AddSkill(blade);
}

// 別処理か別ファイルへ分離予定(JSONもしくはCSVなどで与えるようにするべき)
// ゲーム中に設定で変更できるようにするべき
void GameApp::GameApp::InitializeSkillMappings()
{
	//mSkillInputMap.bind(gNsInput::InputAction::CastSkill1, gNsSkillData::SkillSlot::Primary);// スキルID 1
	//mSkillInputMap.bind(gNsInput::InputAction::CastSkill2, gNsSkillData::SkillSlot::Secondary);// スキルID 2
	//mSkillInputMap.bind(gNsInput::InputAction::CastSkill3, gNsSkillData::SkillSlot::Utility1);// スキルID 3
}