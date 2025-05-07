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
	//logic.rotation = glm::radians(transformComp.rotation.y);
	logic.rotation = transformComp.rotation.y;
	logic.UpdateDirectionFromRotation();

	// Get logical XZ size from model size and drawing scale
	logic.scale = GameUtils::Init::GetModelXZSizeWithScale(transformComp, modelData);

	//std::cout << "[InitLogicTransformSystem.cpp]: Logic Position: x. " << logic.positionXZ.x << " z. " << logic.positionXZ.y << std::endl;
	//std::cout << "[InitLogicTransformSystem.cpp]: logical front vector " << logic.front.x << std::endl;
	//std::cout << "[InitLogicTransformSystem.cpp]: logical right vector " << logic.right.x << std::endl;
	
	DebugUtils::GeneralLog("InitLogicTransformFromModel.cpp", 
		"Logical 2D coordinates were successfully initialized based on model drawing coordinates. ");

	return logic;
}

// A function that creates initialization information for the Tilemap component from the imported BaseMesh model.
// インポートしたBaseMeshモデルからタイルマップコンポーネント(行列構造)の初期化情報を作成する関数
TileMapComponent GameUtils::Init::InitTileMapFromBounds(
	const TransformComponent& transformComp,
	const ModelData& modelData,
	const Logic2DTransformComponent& logic2DComp,
	float tileSize)
{
	TileMapComponent tileMapComp;
	tileMapComp.tileSize = tileSize;

	// 余白タイル(片側当たり)
	const int marginTiles = 1;

	// Get logical XZ size from model size and drawing scale
	glm::vec2 logicalTileMapSize = GameUtils::Init::GetModelWorldSizeWithRotation(transformComp, modelData);

	// Calculate the number of tiles
	// タイル枚数(上下左右に margin を加える)
	tileMapComp.numCols = static_cast<int>(std::ceil(logicalTileMapSize.x / tileSize)) + marginTiles * 2;
	tileMapComp.numRows = static_cast<int>(std::ceil(logicalTileMapSize.y / tileSize)) + marginTiles * 2;

	// Calculate the origin (in world coordinates)
	glm::vec2 baseOrigin = GameUtils::Init::ComputeTileMapOriginFromModel(transformComp, modelData);
	
	// Apply Margin Offset
	// 余白分のオフセットを適用
	tileMapComp.origin = baseOrigin - glm::vec2(marginTiles * tileSize);

	DebugUtils::LogVector("InitLogicTransformFromModel(origin)", tileMapComp.origin);

	return tileMapComp;
}

// 設定済みのTileMapComponentの内容からtiles行列(配列)を初期化する関数
void GameUtils::Init::InitTileMapTiles(TileMapComponent& tileMapComp)
{
	tileMapComp.tiles.resize(tileMapComp.numRows, std::vector<Tile>(tileMapComp.numCols));
	DebugUtils::GeneralLog("InitLogicTransformFromModel(InitTileMapTiles)", "InitTileMapTiles creation completed successfully");
}

glm::vec2 GameUtils::Init::GetModelXZSizeWithScale(const TransformComponent& transformComp, const ModelData& modelData)
{
	glm::vec2 logicalXZSize = glm::vec2(1.0f);

	// モデルデータからサイズを取得
	glm::vec3 rowModelSize = modelData.GetSize();

	// XZサイズをスケール付きで計算
	if (rowModelSize.x > 0.0001f && rowModelSize.z > 0.0001f)
	{
		logicalXZSize.x = transformComp.scale.x * rowModelSize.x;
		logicalXZSize.y = transformComp.scale.z * rowModelSize.z;
	}
	else
	{
		logicalXZSize = glm::vec2(1.0f);
		DebugUtils::GeneralLog("InitLogicTransformFromModel(scale)", "Probably an error occurred while getting/setting the scale.");
	}

	return logicalXZSize;
}


glm::vec2 GameUtils::Init::GetModelWorldSizeWithRotation(const TransformComponent& transformComp, const ModelData& modelData)
{
	glm::vec3 min = modelData.min;
	glm::vec3 max = modelData.max;

	// モデルをワールド空間に変換
	glm::mat4 modelMat = transformComp.toMatrix();
	std::vector<glm::vec3> corners;
	for (int i = 0; i < 8; ++i)
	{
		glm::vec3 local = GetAABBCorner(min, max, i);
		glm::vec4 world = modelMat * glm::vec4(local, 1.0f);// 回転.スケール1.0f
		corners.push_back(glm::vec3(world));
	}

	float minX = corners[0].x, maxX = corners[0].x;
	float minZ = corners[0].z, maxZ = corners[0].z;

	// XZ平面での最小／最大を再計算
	for (auto& c : corners)
	{
		minX = std::min(minX, c.x);
		maxX = std::max(maxX, c.x);
		minZ = std::min(minZ, c.z);
		maxZ = std::max(maxZ, c.z);
	}

	return glm::vec2(maxX - minX, maxZ - minZ);
}

glm::vec3 GameUtils::Init::GetAABBCorner(glm::vec3 min, glm::vec3 max, int i)
{
	// 0:(min.x, min.y, min.z)
	// 1:(max.x, min.y, min.z)
	// 2:(min.x, max.y, min.z)
	// 3:(max.x, max.y, min.z)
	// 4:(min.x, min.y, max.z)
	// 5:(max.x, min.y, max.z)
	// 6:(min.x, max.y, max.z)
	// 7:(max.x, max.y, max.z)
	return glm::vec3(
		(i & 1) ? max.x : min.x,
		(i & 2) ? max.y : min.y,
		(i & 4) ? max.z : min.z
	);
}

glm::vec2 GameUtils::Init::ComputeTileMapOriginFromModel(const TransformComponent& transformComp, const ModelData& modelData)
{
	glm::vec3 min = modelData.min;
	glm::vec3 max = modelData.max;

	glm::mat4 modelMatrix = transformComp.toMatrix();

	float minX = FLT_MAX;
	float minZ = FLT_MAX;

	for (int i = 0; i < 8; ++i)
	{
		glm::vec3 local = GameUtils::Init::GetAABBCorner(min, max, i);
		glm::vec4 world = modelMatrix * glm::vec4(local, 1.0f);

		minX = std::min(minX, world.x);
		minZ = std::min(minZ, world.z);
	}

	return glm::vec2(minX, minZ);
}