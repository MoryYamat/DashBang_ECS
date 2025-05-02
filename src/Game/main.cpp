// entry point

#include <iostream>

#include "Game/Game.h"

#include "Core/ECS/Entity.h"

// main
int main()
{
	//std::cout << "sizeof(Entity): " << sizeof(Entity) << std::endl;

	Game game;

	std::cout << "[main.cpp]: Launching the game..." << std::endl;

	if (game.Initialize())
	{
		game.RunLoop();
	}


	game.Shutdown();

	return 0;
}