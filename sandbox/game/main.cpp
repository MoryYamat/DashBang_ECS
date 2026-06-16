// entry point

// memo

#include <iostream>
#include "game/app.h"
// #include "Game/Game.h"

#include <Windows.h>

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
	auto pid = GetCurrentProcessId();
	std::cout << "Current ProcessID: " << pid << std::endl;


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
// - implement animator_system: asset_manager and ecs orchestration
// - implement A* algo

// MIDDLE
// - implement match index -> dense index (it will improve view performance)
// - implement storage version to the ECS::Storage for caching
// - Correct the design to be fundamentally correct.


// LOW
// - fix cmake and dependencies
// - Creating a custom memory allocator
// - 

// Super LOW:
// - It provides zero-cost abstractions and intuitive type generation and manipulation interfaces for all (semantic) types through logical operations.
// - Editors: camera, asset, semantic validation layer
// - Design and implementation of an IntentLayer that does not include game semantics.

// Folder
// /include: 
// /src:
//   / internal

// System::Update() の 効率化
// 1 全 TransformComponent を走査し、UpdateOne 内で dirty を見る
// 2 View / Query で DirtyTransformComponent を持つ entity だけ走査する
// 3 dirty entity list / changed list を持ち、変更された Transform だけ処理する