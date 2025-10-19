// UIや入力に対応したスキル装備
// 装備リスト
#pragma once

#include <unordered_map>

#include "Game/Combat/Skill/MasterData/SkillSlot.h"



namespace Game::Combat::Skill::Component
{
	// 装備中のスキル(SkillSLotとの対応)
	struct SkillSlotAssignmentComponent
	{
		std::unordered_map<Game::Combat::Skill::Data::SkillSlot, int> slotToSkillId;
	};
}

