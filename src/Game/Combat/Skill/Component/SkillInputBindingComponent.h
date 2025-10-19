//

#pragma once



#include "Game/Input/InputAction.h"
#include "Game/Combat/Skill/MasterData/SkillSlot.h"

#include <unordered_map>

namespace Game::Combat::Skill::Component
{
	struct SkillInputBindingComponent
	{
		std::unordered_map<Game::Input::InputAction, Game::Combat::Skill::Data::SkillSlot> actionToSlot;
	};
}