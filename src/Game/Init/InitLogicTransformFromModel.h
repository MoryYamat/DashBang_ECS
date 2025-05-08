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

	// BaseMesh�f�[�^�A�_���f�[�^�A�ݒ�^�C���T�C�Y����TileMapComponent������������֐�
	TileMapComponent InitTileMapFromBounds(const TransformComponent& transformComp, const ModelData& modelData, const Logic2DTransformComponent& logic2DComp, float tileSize = 1.0f);

	// �ݒ�ς݂�TileMapComponent�̓��e����tiles�s��(�z��)������������֐�
	void InitTileMapTiles(TileMapComponent& tileMapComp);


	// Get logical XZ size from model size, taking into account scale
	// // �X�P�[�����l�����ă��f���T�C�Y����_��XZ�T�C�Y���擾���܂�
	glm::vec2 GetModelXZSizeWithScale(const TransformComponent& transformComp, const ModelData& modelData);

	// ���f���̉�]���l���������[���h���W�ɂ�����ő�^�ŏ��̓_�̋�`���W�����߂�֐�
	glm::vec2 GetModelWorldSizeWithRotation(const TransformComponent& transformComp, const ModelData& modelData);

	// �ő�^�ŏ����W��8�_ ��Ԃ��֐�
	glm::vec3 GetAABBCorner(glm::vec3 min,glm::vec3 max,int i);

	// �_���^�C���}�b�v�̌��_�̃��[���h���W�ɂ�����ʒu�����߂�
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
	// ���f����XZ���ʂɂ�����傫���̍ő厲�𔼌a�ɂ���ꍇ

	float EstimateRadiusFromModelXZ(
		const TransformComponent& transformComp,
		const ModelData& modelData,
		RadiusEstimateStrategy strategy = RadiusEstimateStrategy::MaxAxis
	);
}