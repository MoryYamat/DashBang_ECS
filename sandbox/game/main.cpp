// entry point

// memo

#include <iostream>
#include "app.h"
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
// - Implement Animate Processing
// - apply asset management to existing codes
// - 
// - Implement Skeleton Debug Drawing

// MIDDLE
// - fix folder 
// - Correct the design to be fundamentally correct.

// LOW
// - fix cmake and dependencies


// Super LOW:
// - It provides zero-cost abstractions and intuitive type generation and manipulation interfaces for all (semantic) types through logical operations.


// Folder
// /include: 
// /src:
//   / internal