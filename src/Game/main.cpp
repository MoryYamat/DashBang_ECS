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

// Character /
// Public /
//	MovementApi.hpp
//	MovementFwd.hpp
//	CCApi.hpp
//	CCFwd.hpp
// Private /
//	Control /
//	 Movement /
//    Component /
//		IntentComponents.hpp        // ← ★統合（Facing/MovementIntent）
//		MovementRuntimeComponents.hpp（必要なら）
//	  System /
//		BuildIntent.cpp             // 旧 CharacterMovementIntentResolver.cpp
//		UpdateVelocity.cpp
//		ResolveFacing.cpp
//		System.cpp                  // 司令塔（Movement::Update）
//	CC /
//   Component /
//    CCAntiChainComponent.hpp
//   Policy /
//    CCAntiChainPolicy.hpp
//    CCAntiChainPolicyDatabase.hpp
//   System /
//    CCAntiChainSystem.cpp
