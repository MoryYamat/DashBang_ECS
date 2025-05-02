#include "InitLogicTransformFromModel.h"

#include <iostream>

// Initialize 2D logical data from the position, rotation, and scale of a 3D model
Logic2DTransformComponent GameUtils::Init::InitLogic2DTransformFromModel(const TransformComponent& transform, const ModelData& modelData)
{
	Logic2DTransformComponent logic;

	// �ʒu�f�[�^�̏�����
	logic.positionXZ = glm::vec2(transform.position.x, transform.position.z);

	// ��]�f�[�^�̏�����
	logic.rotation = transform.rotation.y;
	logic.UpdateDirectionFromRotation();

	// �X�P�[���f�[�^�̏����� (���f���T�C�Y�����ƂɌv�Z)
	glm::vec3 modelSize = modelData.GetSize();
	if (modelSize.x > 0.0001f && modelSize.z > 0.0001f)
	{
		logic.scale.x = transform.scale.x * modelSize.x;
		logic.scale.y = transform.scale.z * modelSize.z;
	}
	else
	{
		logic.scale = glm::vec2(1.0f);
	}

	//std::cout << "[InitLogicTransformSystem.cpp]: Logic Position: x. " << logic.positionXZ.x << " z. " << logic.positionXZ.y << std::endl;
	
	return logic;
}