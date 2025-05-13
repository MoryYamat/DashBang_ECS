#include "InitTileMap.h"

#include "Debug/DebugUtils.h"

#include "Game/Init/InitModel/InitLogicTransformFromModel.h"

#include "Core/ECS/Component/Collision/CollisionComponent.h"
#include "Core/ECS/Component/Tags/ObstacleTagComponent.h"

#include "Game/CollisionLogic/CollisionUtils/GeometryUtils.h"

#include <iostream>


// A function that creates initialization information for the Tilemap component from the imported BaseMesh model.
// �C���|�[�g����BaseMesh���f������^�C���}�b�v�R���|�[�l���g(�s��\��)�̏����������쐬����֐�
TileMapComponent GameInit::TileMapFromMesh::InitTileMapFromBounds(
	const TransformComponent& transformComp,
	const ModelData& modelData,
	const Logic2DTransformComponent& logic2DComp,
	float tileSize)
{
	TileMapComponent tileMapComp;
	tileMapComp.tileSize = tileSize;

	// �]���^�C��(�Б�������)
	const int marginTiles = 1;

	// Get logical XZ size from model size and drawing scale
	glm::vec2 logicalTileMapSize = GameInit::TileMapFromMesh::GetModelWorldAABBSizeXZ(transformComp, modelData);

	// Calculate the number of tiles
	// �^�C������(�㉺���E�� margin ��������)
	tileMapComp.numCols = static_cast<int>(std::ceil(logicalTileMapSize.x / tileSize)) + marginTiles * 2;
	tileMapComp.numRows = static_cast<int>(std::ceil(logicalTileMapSize.y / tileSize)) + marginTiles * 2;

	// Calculate the origin (in world coordinates)
	// �_���^�C���}�b�v�̌��_�̃��[���h���W�ɂ�����ʒu
	glm::vec2 baseOrigin = GameInit::TileMapFromMesh::ComputeTileMapOriginFromModel(transformComp, modelData);

	// Apply Margin Offset
	// �]�����̃I�t�Z�b�g��K�p
	tileMapComp.origin = baseOrigin - glm::vec2(marginTiles * tileSize);

	DebugUtils::LogVector("InitLogicTransformFromModel(origin)", tileMapComp.origin);

	return tileMapComp;
}

glm::vec2 GameInit::TileMapFromMesh::GetModelWorldAABBSizeXZ(const TransformComponent& transformComp, const ModelData& modelData)
{
	glm::vec3 min = modelData.min;
	glm::vec3 max = modelData.max;

	// ���f�������[���h��Ԃɕϊ�
	glm::mat4 modelMat = transformComp.toMatrix();
	std::vector<glm::vec3> corners;
	for (int i = 0; i < 8; ++i)
	{
		glm::vec3 local = GameInit::TileMapFromMesh::GetAABBCorner(min, max, i);
		glm::vec4 world = modelMat * glm::vec4(local, 1.0f);// ��].�X�P�[��1.0f
		corners.push_back(glm::vec3(world));
	}

	float minX = corners[0].x, maxX = corners[0].x;
	float minZ = corners[0].z, maxZ = corners[0].z;

	// XZ���ʂł̍ŏ��^�ő���Čv�Z
	for (auto& c : corners)
	{
		minX = std::min(minX, c.x);
		maxX = std::max(maxX, c.x);
		minZ = std::min(minZ, c.z);
		maxZ = std::max(maxZ, c.z);
	}

	return glm::vec2(maxX - minX, maxZ - minZ);
}


// �ݒ�ς݂�TileMapComponent�̓��e����tiles�s��(�z��)������������֐�
void GameInit::TileMapFromMesh::InitTileMapTiles(TileMapComponent& tileMapComp)
{
	tileMapComp.tiles.resize(tileMapComp.numRows, std::vector<Tile>(tileMapComp.numCols));
	DebugUtils::GeneralLog("InitLogicTransformFromModel(InitTileMapTiles)", "InitTileMapTiles creation completed successfully");
}


glm::vec3 GameInit::TileMapFromMesh::GetAABBCorner(glm::vec3 min, glm::vec3 max, int i)
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

glm::vec2 GameInit::TileMapFromMesh::ComputeTileMapOriginFromModel(const TransformComponent& transformComp, const ModelData& modelData)
{
	glm::vec3 min = modelData.min;
	glm::vec3 max = modelData.max;

	glm::mat4 modelMatrix = transformComp.toMatrix();

	float minX = FLT_MAX;
	float minZ = FLT_MAX;

	for (int i = 0; i < 8; ++i)
	{
		glm::vec3 local = GameInit::TileMapFromMesh::GetAABBCorner(min, max, i);
		glm::vec4 world = modelMatrix * glm::vec4(local, 1.0f);

		minX = std::min(minX, world.x);
		minZ = std::min(minZ, world.z);
	}

	return glm::vec2(minX, minZ);
}


void GameInit::TileMapFromMesh::ApplyObstacleCollidersToTileMap(ECS& ecs, TileMapComponent& tileMapComp)
{
	// std::cout << "this function is called " << std::endl;
	for (Entity e : ecs.view<CollisionComponent, ObstacleTagComponent>())
	{
		const auto& collisionComp = ecs.get<CollisionComponent>(e);

		if (collisionComp.collider.type != ColliderType::Obb2D)
			continue;

		const Obb2D& obb = collisionComp.collider.obb2D;

		for (int row = 0; row < tileMapComp.numRows; ++row)
		{
			for(int col = 0; col < tileMapComp.numCols; ++col)
			{
				glm::vec2 tileCenter = tileMapComp.GetTileCenter(row, col);

				auto [tileMin, tileMax] = tileMapComp.GetTileAABB(row, col);
				//if (obb.contains(tileCenter))
				//{
				//	tileMapComp.tiles[row][col].isWalkable = false;

				//	//std::cout << "[InitTileMap.cpp(This Tile is Un Walkable)] row: " << row
				//	//	<< "col: " << col << std::endl;
				//	DebugUtils::LogVector_string("[InitTileMap.cpp(This Tile is Un Walkable)] :", tileCenter);
				//}

				if (GameUtils::CollisionLogic::GeometryUtils::intersectOBB2D_AABB2D(obb, tileMin, tileMax))
				{
					tileMapComp.tiles[row][col].isWalkable = false;
				}
				//std::cout << "[InitTileMap.cpp(WalkableByTerrain)] row:" << row
				//	<< " col:" << col
				//	<< " Result :" << tileMapComp.tiles[row][col].isWalkable << std::endl;

			}
		}
	}
}

void GameInit::TileMapFromMesh::MaskUncoveredTilesByTerrainOBB(ECS& ecs, TileMapComponent& tileMapComp)
{
	for (Entity e : ecs.view<CollisionComponent, TileMapComponent>())
	{
		const auto& collisionComp = ecs.get<CollisionComponent>(e);

		if (collisionComp.collider.type != ColliderType::Obb2D)
			continue;

		const Obb2D& obb = collisionComp.collider.obb2D;

		for (int row = 0; row < tileMapComp.numRows; ++row)
		{
			for (int col = 0; col < tileMapComp.numCols; ++col)
			{
				glm::vec2 tileCenter = tileMapComp.GetTileCenter(row, col);

				auto [tileMin, tileMax] = tileMapComp.GetTileAABB(row, col);
				//if (obb.contains(tileCenter))
				//{
				//	tileMapComp.tiles[row][col].isWalkable = false;

				//	//std::cout << "[InitTileMap.cpp(This Tile is Un Walkable)] row: " << row
				//	//	<< "col: " << col << std::endl;
				//	DebugUtils::LogVector_string("[InitTileMap.cpp(This Tile is Un Walkable)] :", tileCenter);
				//}

				// TerrainMesh�Ɣ��Ȃ��󒆕����̃^�C����`unWalkable`�ݒ�
				if (!GameUtils::CollisionLogic::GeometryUtils::intersectOBB2D_AABB2D(obb, tileMin, tileMax))
				{
					tileMapComp.tiles[row][col].isWalkable = false;
				}

				// �O����^�C������`UnWalkable`
				if (row == 0 || row == tileMapComp.numRows - 1 || col == 0 || col == tileMapComp.numCols - 1)
				{
					tileMapComp.tiles[row][col].isWalkable = false;
				}
				//std::cout << "[InitTileMap.cpp(WalkableByTerrain)] row:" << row
				//	<< " col:" << col
				//	<< " Result :" << tileMapComp.tiles[row][col].isWalkable << std::endl;

			}
		}
	}
}

//bool GameInit::TileMapFromMesh::isInsideTerrainDrawBounds(glm::vec2 pointXZ, )
//{
//	
//}

//
//void GameInit::TileMapFromMesh::InitWalKableByTerrain(TileMapComponent& tileMapComp, const ModelData& modelData)
//{
//	for (int row = 0; row < tileMapComp.numRows; ++row)
//	{
//		for (int col = 0; col < tileMapComp.numCols; ++col)
//		{
//			glm::vec2 center = tileMapComp.GetTileCenter(row, col);
//			bool inside = isInsideTerrainDrawBounds(center, modelData);
//			tileMapComp.tiles[row][col].isWalkable = inside;
//
//			std::cout << "[InitTileMap.cpp(WalkableByTerrain)] row:" << row << " col:" << col << "Result :" << inside << std::endl;
//		}
//	}
//}


// For Obstacles

