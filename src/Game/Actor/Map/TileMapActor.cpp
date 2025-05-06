#include "TileMapActor.h"

#include "Core/ECS/Entity.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include "Debug/DebugUtils.h"

TileMapActor::TileMapActor(ECS& ecs)
{
	Entity entity = ecs.createEntity();

	TileMapComponent tileMapComp;

	// �z��̏��ݒ�
	tileMapComp.numCols = 10;
	tileMapComp.numRows = 10;

	// ���̑��̏��
	tileMapComp.tileSize = 0.5f;
	tileMapComp.origin = glm::vec2(tileMapComp.numCols * tileMapComp.tileSize * -0.5f, tileMapComp.numRows * tileMapComp.tileSize * -0.5f);

	// �z��쐬
	tileMapComp.tiles.resize(tileMapComp.numRows, std::vector<Tile>(tileMapComp.numCols));

	ecs.addComponent(entity, tileMapComp);

	DebugUtils::GeneralLog("TileMapActor.cpp", "Tilemap creation completed successfully");
}