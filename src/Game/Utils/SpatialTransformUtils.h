// Space transformation utility functions for game logic
// �Q�[�����W�b�N�p�̋�ԕϊ��nUtility�֐��Q
#pragma once

#include <GLM/glm.hpp>

namespace GameUtils::SpatialTransform
{
	// �}�E�X�|�C���^(�X�N���[�����W)����A�_��XZ���ʏ�ŃL�����N�^�[�����_�Ƃ������Έʒu�x�N�g���𓾂�
	// Get the relative position vector from the mouse pointer (screen coordinates) to the character's origin on the logical XZ plane
	glm::vec2 MouseTo2DLogicConverter(
		const glm::vec2& screenMousePos,// mouse position
		const glm::mat4& viewMatrix,//  view matrix
		const glm::mat4& projectionMatrix,// projection matrix
		const glm::vec4& viewport,// viewport setting
		const glm::vec2& playerLogicPosXZ // Character position in logical coordinates
		);
}