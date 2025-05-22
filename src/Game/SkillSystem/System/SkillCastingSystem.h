// Actually process the SKillInstance based on the existing SkillInstance and the SkillDefinition of the corresponding SkillID
// ���݂���SkillInstance�ƊY����SkillID��SkillDefinition�Ɋ�Â���SKillInstance�����ۂɏ�������

#pragma once

#include "Core/ECS/EntityManager.h"

#include "Game/Input/InputMapping.h"

#include "Graphics/Renderer/RenderContext.h"


#include "Game/SkillSystem/MasterData/SkillDatabase.h"

namespace SkillSystem::Casting
{
	// �X�L���̘_���I�`��iHitArea�j���`�ɏ]���Đ�������
	void SpawnSkillHitArea(ECS& ecs, SkillDatabase& skillDB);

	void RotateVec2(const glm::vec2& v, float radians);
}