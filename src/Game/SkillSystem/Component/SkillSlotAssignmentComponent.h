// UI����͂ɑΉ������X�L������
// �������X�g
#pragma once

#include <unordered_map>

#include "Game/SkillSystem/MasterData/SkillSlot.h"

// �������̃X�L��(SkillSLot�Ƃ̑Ή�)
struct SkillSlotAssignmentComponent
{
	std::unordered_map<SkillSystem::SkillSlot, int> slotToSkillId;
};