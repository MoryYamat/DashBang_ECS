#pragma once

#include "Game/Combat/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMDefinition.hpp"
#include "Game/Combat/Skill/FSM/Trigger/ISkillTriggerCondition.hpp"

namespace Game::Combat::Skill::Data
{
	using namespace Game::Combat::Skill::FSM::Condition;

	struct SkillEntry
	{
		Game::Combat::Skill::Def::SkillDef def;
		Game::Combat::Skill::FSM::SkillFSMDefinition fsm;
		std::shared_ptr<ISkillTriggerCondition> triggerCondition = nullptr;// (None->Casting‚É“ü‚éğŒ)
	};

}