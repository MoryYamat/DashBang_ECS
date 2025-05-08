// Position data used for logical calculations such as collision detection

#pragma once 

#include <glm/glm.hpp>


struct Logic2DTransformComponent
{
	//position (x, z)
	glm::vec2 positionXZ;

	// direction
	float rotation = 0.0f;

	// Front and right vectors
	glm::vec2 front = glm::vec2(0.0f, 1.0f);
	glm::vec2 right = glm::vec2(1.0f, 0.0f);

	// scale
	glm::vec2 scale = glm::vec2(1.0f);

	// Front�x�N�g�������]�𓾂�
	float GetRotationYFromFrontVector() const
	{
		// ����R�X�g��...
		if (glm::length(front) > 0.0001f)
		{
			return glm::degrees(std::atan2(front.x, front.y));
		}
		else
		{
			return rotation;
		}
	}

	// Front�x�N�g������Right�x�N�g���𓾂�
	void UpdateRightFromFront()
	{
		right = glm::vec2(front.y, -front.x);
	}

	// ���������̂݁@���Q�[�����[�v�ł͎g��Ȃ�
	void UpdateDirectionFromRotation()
	{
		float rad = glm::radians(rotation);
		front = glm::normalize(glm::vec2(glm::sin(rad), glm::cos(rad)));
		right = glm::vec2(front.y, -front.x);
	}
};