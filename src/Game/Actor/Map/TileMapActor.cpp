#include "TileMapActor.h"

#include "Core/ECS/Entity.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include "Debug/DebugUtils.h"

TileMapActor::TileMapActor(ECS& ecs)
{
	Entity entity = ecs.createEntity();

	TileMapComponent tileMapComp;

	// 配列の情報設定
	tileMapComp.numCols = 10;
	tileMapComp.numRows = 10;

	// その他の情報
	tileMapComp.tileSize = 0.5f;
	tileMapComp.origin = glm::vec2(tileMapComp.numCols * tileMapComp.tileSize * -0.5f, tileMapComp.numRows * tileMapComp.tileSize * -0.5f);

	// 配列作成
	tileMapComp.tiles.resize(tileMapComp.numRows, std::vector<Tile>(tileMapComp.numCols));

	ecs.addComponent(entity, tileMapComp);

	DebugUtils::GeneralLog("TileMapActor.cpp", "Tilemap creation completed successfully");
}