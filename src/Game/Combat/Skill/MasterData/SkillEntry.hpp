#pragma once

#include "Game/Combat/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMDefinition.hpp"
#include "Game/Combat/Skill/FSM/Trigger/ISkillTriggerCondition.hpp"

namespace Game::Combat::Skill::Data
{

	struct SkillEntry
	{
		Game::Combat::Skill::Def::SkillDef def;
		Game::Combat::Skill::FSM::SkillFSMDefinition fsm;
		std::shared_ptr<Game::Combat::Skill::FSM::Condition::ISkillTriggerCondition> triggerCondition = nullptr;// (None->Castingに入る条件)
	};

}