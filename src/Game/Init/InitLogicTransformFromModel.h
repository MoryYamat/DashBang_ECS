// Initialize logical data from drawing data

#pragma once

#include "DataTypes/ModelData.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"

#include "Core/ECS/Component/TransformComponent.h"

#include "Core/ECS/Component/TileMapComponent.h"

namespace GameUtils::Init
{
	// 3D -> 2D
	Logic2DTransformComponent InitLogic2DTransformFromModel(const TransformComponent& transformComp, const ModelData& modelData);

	// 3D -> 3D

	// 2D -> 2D

	// BaseMesh�f�[�^�A�_���f�[�^�A�ݒ�^�C���T�C�Y����TileMapComponent������������֐�
	TileMapComponent InitTileMapFromBounds(const TransformComponent& transformComp, const ModelData& modelData, const Logic2DTransformComponent& logic2DComp, float tileSize = 1.0f);

	// �ݒ�ς݂�TileMapComponent�̓��e����tiles�s��(�z��)������������֐�
	void InitTileMapTiles(TileMapComponent& tileMapComp);
}