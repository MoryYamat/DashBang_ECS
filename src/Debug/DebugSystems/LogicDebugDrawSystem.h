// Debug drawing functions
#pragma once

#include "Core/ECS/EntityManager.h"
#include "Graphics/Renderer/RenderContext.h"

namespace LogicDebugDrawSystem
{
	void Draw(ECS& ecs, const RenderContext& renderContext);

	// Player�A�N�^�[�̈ʒu�E�}�E�X�|�C���^�̈ʒu��`��(�f�o�b�O�p)
	void DebugDrawLogicPlayerPositions(ECS& ecs, const RenderContext& renderContext);

	// �^�C���}�b�v�̃��C���t���[���`��(�f�o�b�O�p)
	void DebugDrawLogicTileMaps(ECS& ecs, const RenderContext& renderContext);

	// 2D�~��`��(�f�o�b�O�p)
	void DebugDrawPlayerCollision(ECS& ecs, const RenderContext& renderContext);

	// TileMap��PlayerCircle2D�̃R���W������`��
	void DebugDrawPlayerAndTileMap(ECS& ecs, const RenderContext& renderContext);

	// �����_�[�R���e�L�X�g���Z�b�g�֐�
	void SetOpenGLMatrixState(const RenderContext& renderContext);

	// �����_�[�R���e�L�X�g����������֐�
	void ResetOpenGLMatrixState();
}