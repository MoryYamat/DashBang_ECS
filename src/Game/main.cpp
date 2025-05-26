// entry point

// 描画(3D) 右手系.X/Y/Z
// 論理(2D) 右手系.X/-Z
// 描画と論理はY軸周りに180°ズレている（処理中に補正が必要）

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