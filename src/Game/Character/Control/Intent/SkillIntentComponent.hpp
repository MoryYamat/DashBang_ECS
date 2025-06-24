// キャラクター制御意図

#pragma once

#include "Game/Combat/Skill/MasterData/SkillSlot.h"

#include "Common/GameNamespaceDecl.h"

#include <vector>


namespace Game::Character::Control
{
	struct SkillIntentComponent
	{
		std::vector<gNsSkillData::SkillSlot> requestedSlots;
		bool isActive = false;
	};
}