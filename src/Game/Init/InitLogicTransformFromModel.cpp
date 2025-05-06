#include "InitLogicTransformFromModel.h"

#include "Debug/DebugUtils.h"


#include <iostream>

// Initialize 2D logical data from the position, rotation, and scale of a 3D model
Logic2DTransformComponent GameUtils::Init::InitLogic2DTransformFromModel(const TransformComponent& transformComp, const ModelData& modelData)
{
	Logic2DTransformComponent logic;

	// �ʒu�f�[�^�̏�����
	logic.positionXZ = glm::vec2(transformComp.position.x, transformComp.position.z);

	// ��]�f�[�^�̏����� (�ǂ�����ă��f�����ʂ𓾂邩)
	logic.rotation = transformComp.rotation.y;
	logic.UpdateDirectionFromRotation();

	// �X�P�[���f�[�^�̏����� (���f���T�C�Y�����ƂɌv�Z)
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
// �C���|�[�g����BaseMesh���f������^�C���}�b�v�R���|�[�l���g�̏����������쐬����֐�
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

// �ݒ�ς݂�TileMapComponent�̓��e����tiles�s��(�z��)������������֐�
void GameUtils::Init::InitTileMapTiles(TileMapComponent& tileMapComp)
{
	tileMapComp.tiles.resize(tileMapComp.numRows, std::vector<Tile>(tileMapComp.numCols));
	DebugUtils::GeneralLog("InitLogicTransformFromModel(InitTileMapTiles)", "InitTileMapTiles creation completed successfully");
}