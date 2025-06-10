// Initialize logical data from drawing data

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/Graphics/Model/ModelData.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include <string>

#include "Common/EngineNamespaceDecl.h"

namespace Game::Init::Logic2D
{
	// BaseMeshデータ、論理データ、設定タイルサイズからTileMapComponentを初期化する関数
	eNsLogic2DComp::TileMapComponent InitTileMapFromBounds(
		const eNsCommonComp::TransformComponent& transformComp
		, const eNsGfxModel::ModelData& modelData
		, const eNsLogic2DComp::Logic2DTransformComponent& logic2DComp
		, float tileSize = 1.0f);

	// 設定済みのTileMapComponentの内容からtiles行列(配列)を初期化する関数
	void InitTileMapTiles(eNsLogic2DComp::TileMapComponent& tileMapComp);

	// モデルの回転を考慮したワールド座標における最大／最小の点の矩形座標を求める関数
	glm::vec2 GetModelWorldAABBSizeXZ(const eNsCommonComp::TransformComponent& transformComp, const eNsGfxModel::ModelData& modelData);

	// 最大／最小座標の8点 を返す関数
	glm::vec3 GetAABBCorner(glm::vec3 min, glm::vec3 max, int i);

	// 論理タイルマップの原点のワールド座標における位置を求める
	glm::vec2 ComputeTileMapOriginFromModel(const eNsCommonComp::TransformComponent& transformComp, const eNsGfxModel::ModelData& modelData);


	// 中心点がTerrainMeshのAABB内にあるかチェックする用の関数
	//bool isInsideTerrainDrawBounds(glm::vec2 pointXZ, const ModelData& modelData);

	// to init to walkable attribution by terrain mesh
	//void InitWalKableByTerrain(TileMapComponent& tileMapComp, const ModelData& modelData);
}

// For set tile attributes
namespace Game::Init::Logic2D
{
	// 障害物アクターとかぶるタイルマップの属性を`unWalkable`へ設定する
	void ApplyObstacleCollidersToTileMap(eNsECS::EntityMgr& ecs, eNsLogic2DComp::TileMapComponent& tileMapComp);

	// 中空にあるタイルマップの属性を`unWalkable`へ設定
	void MaskUncoveredTilesByTerrainOBB(eNsECS::EntityMgr& ecs, eNsLogic2DComp::TileMapComponent& tileMapComp);
}