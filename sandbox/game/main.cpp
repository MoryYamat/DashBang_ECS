// entry point

// memo

#include <iostream>
#include "game/app.h"
// #include "Game/Game.h"

// #include <Windows.h>

// main
int main()
{
	// GameApp::GameApp game;

	// std::cout << "[main.cpp]: Launching the game..." << std::endl;

	// if (game.Initialize())
	// {
	// 	game.RunLoop();
	// }

	// game.Shutdown();

	// pid get
	// auto pid = GetCurrentProcessId();
	// std::cout << "Current ProcessID: " << pid << std::endl;


	std::cout<<"hello world\n";

	app::App app;

	if(!app.Init(720,480))
	{
		std::cerr << "app init failed \n"; 
	}
	else
	{
		app.Loop();
	}




	return 0;

}

// ========= TODO: ========= 
// HIGH
// - add external animation clip importer
// - ボタンによる簡易攻撃を一つだけ実装、animation再生と、hitobox生成を含む、そしてhitboxとその交差判定のデバッグ描画．debug表示として、character entityの状態やcomponent数などの表示
// - implement animator_system: asset_manager and ecs orchestration
// - implement A* algo
// - add simple directional light and lambert diffuse

// MIDDLE
// - glTF node hierarchy と runtime bone hierarchy が 1:1 ではないことの対応
// - implement match index -> dense index (it will improve view performance)
// - implement storage version to the ECS::Storage for caching
// - Creating a custom memory allocator
// - Correct the design to be fundamentally correct.
// - Improve the vertex data definition and import implementation of glTFImportData (-> Align the granularity with texture and image data) 
// - Flag systems such as attack intent requests can potentially be optimized using tools like bitflag.
// - glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED); の Windowns / Linux 環境差の 調査


// LOW
// - fix cmake and dependencies
// - extends rendering system (state manager/pipeline manager/ lightsystem/ ...)

// Super LOW:
// - It provides zero-cost abstractions and intuitive type generation and manipulation interfaces for all (semantic) types through logical operations.
// - Editors: camera, asset, semantic validation layer
// - Design and implementation of an IntentLayer that does not include game semantics.
// - add render state manager

// Folder
// /include: 
// /src:
//   / internal

// System::Update() の 効率化
// 1 全 TransformComponent を走査し、UpdateOne 内で dirty を見る
// 2 View / Query で DirtyTransformComponent を持つ entity だけ走査する
// 3 dirty entity list / changed list を持ち、変更された Transform だけ処理する


// 作成する資料
// - Readme
// 

// - overview: プロジェクトの目的/何が動くか/何を自作したか/全体アーキテクチャ図/debug mode のスクショ/demovideo のリンク/ 詳細docsへのリンク
// - ECS : 現行設計/ 
// - AssetManager: 
// - import pipeline: 
// - SystemPipeline(fsm-intent design)(input->request->state->resolver->Logic): 
// - DebugTools
// - appendix: FSMEngine(現行のものには未使用ですが以前のもの使用) 
// 
// - Demo 動画
// 