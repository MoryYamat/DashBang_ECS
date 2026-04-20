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

	app::App app(720,480);

	if(!app.Init())
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
// - apply asset management to existing codes
// - create debug / test codes
// - 

// MIDDLE
// - fix folder 

// LOW
// - fix cmake and dependencies



// Folder
// /include: 
// /src:
//   / internal