// 

#pragma once

#include "Game/Combat/Skill/MasterData/SkillSlot.h"



#include <vector>

// 削除予定：未使用 -> Character側のIntentに移行
namespace Game::Combat::Skill::Intent
{
	struct SkillIntentComponent
	{
		std::vector<Game::Combat::Skill::Data::SkillSlot> requestedSlots;
		bool isActive = false;
	};
}
