// The system responsible for "executing" hit detection
// 当たり判定の「実行」責任を持つシステム

#pragma once 

#include "Engine/ECS/EntityManager.h"

#include "Game/Collision/Data/CollisionResultStorage.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"
// 
namespace Game::Collision::System
{
	void UpdateCollisionResultStorage(eNsECS::EntityMgr& ecs, gNsCollData::CollisionResultStorage& collisionResultStorage);


	void CollisionDetectionSystem(eNsECS::EntityMgr& ecs, gNsCollData::CollisionResultStorage& collisionResultStorage);


	// player and tile map collision detection

}