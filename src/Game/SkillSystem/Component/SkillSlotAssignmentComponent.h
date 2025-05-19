// UIや入力に対応したスキル装備
// 装備リスト
#pragma once

#include <unordered_map>

#include "Game/SkillSystem/MasterData/SkillSlot.h"

// 装備中のスキル(SkillSLotとの対応)
struct SkillSlotAssignmentComponent
{
	std::unordered_map<SkillSystem::SkillSlot, int> slotToSkillId;
};