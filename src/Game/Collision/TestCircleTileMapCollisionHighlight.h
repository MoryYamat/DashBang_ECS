// Write a process for tiles that collide with the player character


#pragma once

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Common/EngineNamespaceDecl.h"

//namespace Game::Collision::Utils
//{
//	// Scan the circle Collider and all tiles in the TileMap, and do something to the tiles that are in contact.
//	// 円ColliderとTileMapの全タイルとを走査し、接触しているタイルに対して何か処理をする
//	void TestCircleTileMapCollisionHighlight(
//		const eNsLogic2DComp::CollisionComponent& colliderComp,
//		const eNsLogic2DComp::TileMapComponent& tileMapComp
//	);
//}