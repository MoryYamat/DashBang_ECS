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
	// BaseMesh�f�[�^�A�_���f�[�^�A�ݒ�^�C���T�C�Y����TileMapComponent������������֐�
	TileMapComponent InitTileMapFromBounds(const TransformComponent& transformComp, const ModelData& modelData, const Logic2DTransformComponent& logic2DComp, float tileSize = 1.0f);

	// �ݒ�ς݂�TileMapComponent�̓��e����tiles�s��(�z��)������������֐�
	void InitTileMapTiles(TileMapComponent& tileMapComp);

	// ���f���̉�]���l���������[���h���W�ɂ�����ő�^�ŏ��̓_�̋�`���W�����߂�֐�
	glm::vec2 GetModelWorldAABBSizeXZ(const TransformComponent& transformComp, const ModelData& modelData);

	// �ő�^�ŏ����W��8�_ ��Ԃ��֐�
	glm::vec3 GetAABBCorner(glm::vec3 min, glm::vec3 max, int i);

	// �_���^�C���}�b�v�̌��_�̃��[���h���W�ɂ�����ʒu�����߂�
	glm::vec2 ComputeTileMapOriginFromModel(const TransformComponent& transformComp, const ModelData& modelData);


	// ���S�_��TerrainMesh��AABB���ɂ��邩�`�F�b�N����p�̊֐�
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