// Write a process for tiles that collide with the player character


#pragma once

#include "Core/ECS/Component/Collision/CollisionComponent.h"

#include "Core/ECS/Component/TileMapComponent.h"

namespace CollisionUtils
{
	// Scan the circle Collider and all tiles in the TileMap, and do something to the tiles that are in contact.
	// �~Collider��TileMap�̑S�^�C���Ƃ𑖍����A�ڐG���Ă���^�C���ɑ΂��ĉ�������������
	void TestCircleTileMapCollisionHighlight(
		const CollisionComponent& colliderComp,
		const TileMapComponent& tileMapComp
	);
}