// Skill�̃��C�t�^�C�����CCaster�CID��ێ��D
// 

#pragma once 

#include "Core/ECS/Entity.h"

#include <cstdint>

// Active Skill �g�p���ɕt�^�����f�[�^
// Data granted when using Active Skill
struct SkillInstanceComponent
{
	Entity caster;

	float timeSinceCast = 0.0f;

	uint16_t skillId;

	// �X�L��ID�Ȃǂ�����
};