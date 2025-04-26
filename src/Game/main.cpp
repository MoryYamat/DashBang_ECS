//

#include <iostream>

#include "Game/Game.h"

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