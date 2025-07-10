#pragma once


#include <cstdint>

#include "Game/Combat/Skill/FSM/Definition/SkillTransitionTable.hpp"

namespace Game::Combat::Skill::FSM
{

	struct SkillFSMDefinition
	{
		uint32_t id;
		std::vector<SkillTransition> transitions;
	};
}