// Space transformation utility functions for game logic
// �Q�[�����W�b�N�p�̋�ԕϊ��nUtility�֐��Q
#pragma once

#include <GLM/glm.hpp>

#include "Graphics/Renderer/RenderContext.h"

namespace GameUtils::SpatialTransform
{
	// �}�E�X�|�C���^(�X�N���[�����W)����A�_��XZ���ʏ�ŃL�����N�^�[�����_�Ƃ������Έʒu�x�N�g���𓾂�
	// Get the relative position vector from the mouse pointer (screen coordinates) to the character's origin on the logical XZ plane
	glm::vec2 GetLogicDirectionFromCursorToTarget(const glm::vec2& screenMousePos, const RenderContext& renderContext,
		const glm::vec2& playerLogicPosXZ // Character position in logical coordinates
		);

	// �X�N���[�����W(�}�E�X�|�C���^�Ȃ�)����A�_��XZ���ʂł̍��W�𓾂�
	glm::vec2 ProjectScreenToLogicXZPlane(const glm::vec2& screenMousePos, const RenderContext& renderContext);
}