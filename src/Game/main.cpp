// entry point

#include <iostream>

#include "Game/Game.h"

// main
int main()
{
	Game game;

	std::cout << "[main.cpp]: Launching the game..." << std::endl;

	if (game.Initialize())
	{
		game.RunLoop();
	}

	game.Shutdown();

	return 0;
}