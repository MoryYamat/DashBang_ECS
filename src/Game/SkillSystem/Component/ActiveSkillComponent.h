// Active Skill Component

#pragma once 

#include "Core/ECS/Entity.h"

// Active Skill �g�p���ɕt�^�����f�[�^
// Data granted when using Active Skill
struct ActiveSkillComponent
{
	Entity caster;

	float timeSinceCast = 0.0f;

	// �X�L��ID�Ȃǂ�����
};