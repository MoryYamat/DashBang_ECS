// A system that generates player character skills based on input
// ���͂����ƂɃv���C���[�L�����N�^�[�̃X�L���𐶐�����V�X�e��

#pragma once

#include "Core/ECS/EntityManager.h"

#include "Game/Input/InputState.h"

namespace SkillSystem
{
	void PlayerSkillSpawnSystem(ECS& ecs, InputState& input);
}