// Debug drawing functions
#pragma once

#include "Core/ECS/EntityManager.h"
#include "Graphics/Renderer/RenderContext.h"

#include "Game/CollisionLogic/Data/CollisionResultStorage.h"

namespace LogicDebugDrawSystem
{
	// �f�o�b�O�p�`��̃C���^�[�t�F�[�X
	void Draw(ECS& ecs,
		const RenderContext& renderContext,
		const CollisionResultStorage collisionResult);

	// Player�A�N�^�[�̈ʒu�E�}�E�X�|�C���^�̈ʒu��`��(�f�o�b�O�p)
	void DebugDrawLogicPlayerPositions(ECS& ecs, const RenderContext& renderContext);

	// �^�C���}�b�v�̃��C���t���[���`��(�f�o�b�O�p)
	void DebugDrawLogicTileMaps(ECS& ecs, const RenderContext& renderContext);

	// 2D�~��`��(�f�o�b�O�p)
	void DebugDrawPlayerCollision(ECS& ecs, const RenderContext& renderContext);

	// TileMap��PlayerCircle2D�̃R���W������`��
	void DebugDrawPlayerAndTileMap(ECS& ecs,
		const RenderContext& renderContext,
		const CollisionResultStorage collisionResult);

	// �����_�[�R���e�L�X�g���Z�b�g�֐�
	void SetOpenGLMatrixState(const RenderContext& renderContext);

	// �����_�[�R���e�L�X�g����������֐�
	void ResetOpenGLMatrixState();

}

// �t�@�C�����u�V�X�e���p�f�o�b�O�`��v�Ɓu�Q�[���p�f�o�b�O�`��v�ɕ�����
// �t�@�C�����u�V�X�e���p�f�o�b�O�`��v�Ɓu�Q�[���p�f�o�b�O�`��v�ɕ�����
namespace LogicDebugDrawSystem::SkillDaraw
{
	// Skill�̘_���I���ʔ͈͌`���`�悷��
	void RenderAttack2DAreas(ECS& ecs, const RenderContext& renderContext);
}