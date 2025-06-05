// 

#pragma once

#include "Game/Combat/Skill/MasterData/SkillSlot.h"

#include "Common/GameNamespaceDecl.h"

#include <vector>


namespace Game::Combat::Skill::Intent
{
	struct SkillIntentComponent
	{
		std::vector<gNsSkillData::SkillSlot> requestedSlots;
		bool isActive = false;
	};
}
