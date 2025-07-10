#pragma once

#include "Game/Combat/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Skill/FSM/Definition/SkillFSMDefinition.hpp"

namespace Game::Combat::Skill::Data
{
	struct SkillEntry
	{
		Game::Combat::Skill::Def::SkillDef def;
		Game::Combat::Skill::FSM::SkillFSMDefinition fsm;
	};

}