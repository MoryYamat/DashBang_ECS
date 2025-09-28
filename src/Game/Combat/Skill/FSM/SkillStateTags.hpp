#pragma once


#include "Game/Combat/Skill/FSM/StateModel/SkillFSMStates.hpp"

#include <typeindex>

namespace Game::Combat::Skill::FSM::AxisTag
{
	inline const std::type_index SKILLAXIS = typeid(Game::Combat::Skill::FSM::SkillFSMAxis);
}

namespace Game::Combat::Skill::FSM::StateTag
{ 

	inline const std::type_index NONE = typeid(Game::Combat::Skill::FSM::SkillPhase::None);
	inline const std::type_index CASTING = typeid(Game::Combat::Skill::FSM::SkillPhase::Casting);
	inline const std::type_index ACTIVE = typeid(Game::Combat::Skill::FSM::SkillPhase::Active);
	inline const std::type_index RECOVERY = typeid(Game::Combat::Skill::FSM::SkillPhase::Recovery);
	inline const std::type_index COMPLETED = typeid(Game::Combat::Skill::FSM::SkillPhase::Completed);
	inline const std::type_index INTERRUPTED = typeid(Game::Combat::Skill::FSM::SkillPhase::Interrupted);
}