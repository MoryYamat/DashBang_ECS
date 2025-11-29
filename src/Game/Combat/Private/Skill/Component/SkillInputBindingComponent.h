//

#pragma once




#include "Game/Combat/Private/Skill/MasterData/SkillSlot.h"

#include <unordered_map>

namespace Game::Combat::Skill::Component
{
	struct SkillInputBindingComponent
	{
		std::unordered_map<Game::Input::InputAction, Game::Combat::Skill::Data::SkillSlot> actionToSlot;
	};
}