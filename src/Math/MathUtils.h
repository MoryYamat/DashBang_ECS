// math

#pragma once

#include <glm/glm.hpp>

namespace MathUtils
{
	// ����: ����� XZ���ʏ�Ŏg�p������]�ł��iZ = -Y�j
	inline glm::vec2 RotateVec2_XZ(const glm::vec2& v, float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::vec2(
			c * v.x - s * v.y,
			-(s * v.x + c * v.y)
		);
	}

	// �W���I��2D��]�iXY��ԁj
	inline glm::vec2 RotateVec2(const glm::vec2& v, float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::vec2(
			c * v.x - s * v.y,
			s * v.x + c * v.y
		);
	}

	// ��]�s��x�[�X
	inline glm::mat2 RotationMatrix2D(float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::mat2(c, -s, s, c);
	}
}
