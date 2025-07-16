#pragma once


#include <cstdint>

#include "Game/Combat/Skill/FSM/Definition/SkillTransitionTable.hpp"
#include "Game/Combat/Skill/FSM/Definition/SkillFSMStates.hpp"

#include <typeindex>

namespace Game::Combat::Skill::FSM
{

	struct SkillFSMDefinition
	{
		uint32_t id;
		std::vector<SkillTransition> transitions;
		std::type_index initialState = typeid(SkillPhase::Casting);// –¾¦“I‚É‰Šúó‘Ô‚ğ’è‹`
	};
}