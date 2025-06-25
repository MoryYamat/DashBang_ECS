// 

#pragma once

#include "Game/Combat/Skill/MasterData/SkillSlot.h"

#include "Common/GameNamespaceDecl.h"

#include <vector>

// 削除予定：未使用 -> Character側のIntentに移行
namespace Game::Combat::Skill::Intent
{
	struct SkillIntentComponent
	{
		std::vector<gNsSkillData::SkillSlot> requestedSlots;
		bool isActive = false;
	};
}
