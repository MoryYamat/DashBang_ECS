// entry point

// memo

#include <iostream>

#include "Game/Game.h"

#include "Engine/ECS/Entity.h"


// main
int main()
{
	//std::cout << "sizeof(Entity): " << sizeof(Entity) << std::endl;

	GameApp::GameApp game;

	std::cout << "[main.cpp]: Launching the game..." << std::endl;

	if (game.Initialize())
	{
		game.RunLoop();
	}

	game.Shutdown();

	return 0;

}

