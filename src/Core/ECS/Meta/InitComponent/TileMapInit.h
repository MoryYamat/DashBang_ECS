// Special template for TileMapComponent initialization
// TileMapComponent�������p�̓���e���v���[�g

#pragma once

#include "Core/ECS/Meta/InitComponent/InitSystem.h"

#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include "Game/Init/InitTileMap/InitTileMap.h"

template<>
struct InitSystem<TileMapComponent>
{
	static void Init(TileMapComponent& tileMapComp, ECS& ecs, Entity tileMapEntity)
	{
		GameInit::TileMapFromMesh::ApplyObstacleCollidersToTileMap(ecs, tileMapComp);
		GameInit::TileMapFromMesh::MaskUncoveredTilesByTerrainOBB(ecs, tileMapComp);
	}

};