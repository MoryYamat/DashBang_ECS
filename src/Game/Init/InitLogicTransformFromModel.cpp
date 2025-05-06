#include "InitLogicTransformFromModel.h"

#include "Debug/DebugUtils.h"


#include <iostream>

// Initialize 2D logical data from the position, rotation, and scale of a 3D model
Logic2DTransformComponent GameUtils::Init::InitLogic2DTransformFromModel(const TransformComponent& transformComp, const ModelData& modelData)
{
	Logic2DTransformComponent logic;

	// 位置データの初期化
	logic.positionXZ = glm::vec2(transformComp.position.x, transformComp.position.z);

	// 回転データの初期化 (どうやってモデル正面を得るか)
	logic.rotation = transformComp.rotation.y;
	logic.UpdateDirectionFromRotation();

	// スケールデータの初期化 (モデルサイズをもとに計算)
	glm::vec3 modelSize = modelData.GetSize();
	if (modelSize.x > 0.0001f && modelSize.z > 0.0001f)
	{
		logic.scale.x = transformComp.scale.x * modelSize.x;
		logic.scale.y = transformComp.scale.z * modelSize.z;
	}
	else
	{
		logic.scale = glm::vec2(1.0f);
		DebugUtils::GeneralLog("InitLogicTransformFromModel.cpp", "Probably an error occurred while getting/setting the scale.");
	}

	//std::cout << "[InitLogicTransformSystem.cpp]: Logic Position: x. " << logic.positionXZ.x << " z. " << logic.positionXZ.y << std::endl;
	//std::cout << "[InitLogicTransformSystem.cpp]: logical front vector " << logic.front.x << std::endl;
	//std::cout << "[InitLogicTransformSystem.cpp]: logical right vector " << logic.right.x << std::endl;
	
	DebugUtils::GeneralLog("InitLogicTransformFromModel.cpp", 
		"Logical 2D coordinates were successfully initialized based on model drawing coordinates. ");

	return logic;
}

// A function that creates initialization information for the Tilemap component from the imported BaseMesh model.
// インポートしたBaseMeshモデルからタイルマップコンポーネントの初期化情報を作成する関数
TileMapComponent GameUtils::Init::InitTileMapFromBounds(const TransformComponent& transformComp, const ModelData& modelData, const Logic2DTransformComponent& logic2DComp, float tileSize)
{
	TileMapComponent tileMapComp;

	tileMapComp.tileSize = tileSize;

	glm::vec2 rowTileMapSize = glm::vec2(1.0f);

	// 
	glm::vec3 modelSize = modelData.GetSize();

	DebugUtils::LogVector("InitLogicTransformFromModel(size)", modelSize);
	if (modelSize.x > 0.0001f && modelSize.z > 0.0001f)
	{
		rowTileMapSize.x = transformComp.scale.x * modelSize.x;
		rowTileMapSize.y = transformComp.scale.z * modelSize.z;
	}
	else
	{
		rowTileMapSize = glm::vec2(1.0f);
		DebugUtils::GeneralLog("InitLogicTransformFromModel(scale)", "Probably an error occurred while getting/setting the scale.");
	}

	tileMapComp.numCols = static_cast<int>(rowTileMapSize.x / tileSize);
	tileMapComp.numRows = static_cast<int>(rowTileMapSize.y / tileSize);

	tileMapComp.origin = tileMapComp.GetOrigin(logic2DComp.positionXZ);

	DebugUtils::LogVector("InitLogicTransformFromModel(origin)", tileMapComp.origin);

	return tileMapComp;
}

// 設定済みのTileMapComponentの内容からtiles行列(配列)を初期化する関数
void GameUtils::Init::InitTileMapTiles(TileMapComponent& tileMapComp)
{
	tileMapComp.tiles.resize(tileMapComp.numRows, std::vector<Tile>(tileMapComp.numCols));
	DebugUtils::GeneralLog("InitLogicTransformFromModel(InitTileMapTiles)", "InitTileMapTiles creation completed successfully");
}