// キャラクター制御意図

#pragma once

#include "Game/Combat/Private/Skill/MasterData/SkillSlot.h"


#include <vector>


namespace Game::Character::Control::Skill
{
	struct SkillIntentComponent
	{
		// 要求されたスキルスロット
		std::vector<Game::Combat::Skill::Data::SkillSlot> requestedSlots;

		// ボタンが押されているかどうか
		bool isActive = false;
	};
}