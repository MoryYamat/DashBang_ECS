// The system responsible for "executing" hit detection
// 当たり判定の「実行」責任を持つシステム

#pragma once 

#include "Core/ECS/EntityManager.h"

#include "Game/CollisionLogic/Data/CollisionResultStorage.h"

// 
namespace GameUtils::CollisionLogic::DetectionSystem
{
	void UpdateCollisionResultStorage(ECS& ecs, CollisionResultStorage& collisionResultStorage);


	void CollisionDetectionSystem(ECS& ecs, CollisionResultStorage& collisionResultStorage);


	// player and tile map collision detection

}