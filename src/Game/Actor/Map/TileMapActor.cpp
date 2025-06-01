#include "TileMapActor.h"

#include "Engine/ECS/Entity.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Engine/Debug/DebugUtils.h"

Game::Actor::Map::TileMapActor::TileMapActor(eNsECS::EntityMgr& ecs)
{
	eNsECS::Entity entity = ecs.createEntity();

	eNsLogic2DComp::TileMapComponent tileMapComp;

	// ”z—ñ‚Ìî•ñİ’è
	tileMapComp.numCols = 10;
	tileMapComp.numRows = 10;

	// ‚»‚Ì‘¼‚Ìî•ñ
	tileMapComp.tileSize = 0.5f;
	tileMapComp.origin = glm::vec2(tileMapComp.numCols * tileMapComp.tileSize * -0.5f, tileMapComp.numRows * tileMapComp.tileSize * -0.5f);

	// ”z—ñì¬
	tileMapComp.tiles.resize(tileMapComp.numRows, std::vector<eNsLogic2DComp::Tile>(tileMapComp.numCols));

	ecs.addComponent(entity, tileMapComp);

	eNsDebugLog::GeneralLog("TileMapActor.cpp", "Tilemap creation completed successfully");
}