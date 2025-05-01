// A component that stores data on the minimum and maximum coordinates of the model space.
// ���f����Ԃ̍ŏ����W�A�ő���W�̃f�[�^���i�[����R���|�[�l���g

#include <GLM/glm.hpp>

struct ModelSizeComponent
{
	glm::vec3 min;
	glm::vec3 max;

	// �T�C�Y�̎擾
	glm::vec3 GetSize() const 
	{
		return max - min;
	}

	// ���S���W�̎擾
	glm::vec3 GetCenter() const
	{
		return (min + max) * 0.5f;
	}
};