// Initialize logical data from drawing data

#pragma once

#include "DataTypes/ModelData.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"

#include "Core/ECS/Component/TransformComponent.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include <string>

namespace GameUtils::Init
{
	// 3D -> 2D
	Logic2DTransformComponent InitLogic2DTransformFromModel(const TransformComponent& transformComp, const ModelData& modelData);

	// 3D -> 3D

	// 2D -> 2D

	// BaseMeshデータ、論理データ、設定タイルサイズからTileMapComponentを初期化する関数
	TileMapComponent InitTileMapFromBounds(const TransformComponent& transformComp, const ModelData& modelData, const Logic2DTransformComponent& logic2DComp, float tileSize = 1.0f);

	// 設定済みのTileMapComponentの内容からtiles行列(配列)を初期化する関数
	void InitTileMapTiles(TileMapComponent& tileMapComp);


	// Get logical XZ size from model size, taking into account scale
	// // スケールを考慮してモデルサイズから論理XZサイズを取得します
	glm::vec2 GetModelXZSizeWithScale(const TransformComponent& transformComp, const ModelData& modelData);

	// モデルの回転を考慮したワールド座標における最大／最小の点の矩形座標を求める関数
	glm::vec2 GetModelWorldSizeWithRotation(const TransformComponent& transformComp, const ModelData& modelData);

	// 最大／最小座標の8点 を返す関数
	glm::vec3 GetAABBCorner(glm::vec3 min,glm::vec3 max,int i);

	// 論理タイルマップの原点のワールド座標における位置を求める
	glm::vec2 ComputeTileMapOriginFromModel(const TransformComponent& transformComp, const ModelData& modelData);


}

namespace GameUtils::Init
{
	enum class RadiusEstimateStrategy
	{
		MaxAxis,  // Use the larger of width or depth
		MinAxis,  // Use the smaller of width or depth
		Diagonal      // Use the average of width and depth

	};

	// For initializing collision shape 
	// When the maximum axis of the model's size on the XZ plane is the radius
	// モデルのXZ平面における大きさの最大軸を半径にする場合

	float EstimateRadiusFromModelXZ(
		const TransformComponent& transformComp,
		const ModelData& modelData,
		RadiusEstimateStrategy strategy = RadiusEstimateStrategy::MaxAxis
	);
}