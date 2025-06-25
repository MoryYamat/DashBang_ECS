// キャラクター制御意図

#pragma once

#include "Game/Combat/Skill/MasterData/SkillSlot.h"

#include "Common/GameNamespaceDecl.h"

#include <vector>


namespace Game::Character::Control::Skill
{
	struct SkillIntentComponent
	{
		// 要求されたスキルスロット
		std::vector<gNsSkillData::SkillSlot> requestedSlots;

		// ボタンが押されているかどうか
		bool isActive = false;
	};
}