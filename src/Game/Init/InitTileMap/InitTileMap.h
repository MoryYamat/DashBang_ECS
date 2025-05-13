// Initialize logical data from drawing data

#pragma once

#include "Core/ECS/EntityManager.h"

#include "DataTypes/ModelData.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"

#include "Core/ECS/Component/TransformComponent.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include <string>


namespace GameInit::TileMapFromMesh
{
	// BaseMeshデータ、論理データ、設定タイルサイズからTileMapComponentを初期化する関数
	TileMapComponent InitTileMapFromBounds(const TransformComponent& transformComp, const ModelData& modelData, const Logic2DTransformComponent& logic2DComp, float tileSize = 1.0f);

	// 設定済みのTileMapComponentの内容からtiles行列(配列)を初期化する関数
	void InitTileMapTiles(TileMapComponent& tileMapComp);

	// モデルの回転を考慮したワールド座標における最大／最小の点の矩形座標を求める関数
	glm::vec2 GetModelWorldAABBSizeXZ(const TransformComponent& transformComp, const ModelData& modelData);

	// 最大／最小座標の8点 を返す関数
	glm::vec3 GetAABBCorner(glm::vec3 min, glm::vec3 max, int i);

	// 論理タイルマップの原点のワールド座標における位置を求める
	glm::vec2 ComputeTileMapOriginFromModel(const TransformComponent& transformComp, const ModelData& modelData);


	// 中心点がTerrainMeshのAABB内にあるかチェックする用の関数
	//bool isInsideTerrainDrawBounds(glm::vec2 pointXZ, const ModelData& modelData);

	// to init to walkable attribution by terrain mesh
	//void InitWalKableByTerrain(TileMapComponent& tileMapComp, const ModelData& modelData);
}

// For set tile attributes
namespace GameInit::TileMapFromMesh
{

	void ApplyObstacleCollidersToTileMap(ECS& ecs, TileMapComponent& tileMapComp);

	void MaskUncoveredTilesByTerrainOBB(ECS& ecs, TileMapComponent& tileMapComp);
}