#pragma once


#include "Game/Combat/Skill/FSM/Definition/SkillFSMStates.hpp"

#include <typeindex>

namespace Game::Combat::Skill::FSM::StateTag
{ 
	using namespace Game::Combat::Skill::FSM;

	inline const std::type_index NONE = typeid(SkillPhase::None);
	inline const std::type_index CASTING = typeid(SkillPhase::Casting);
	inline const std::type_index ACTIVE = typeid(SkillPhase::Active);
	inline const std::type_index RECOVERY = typeid(SkillPhase::Recovery);
	inline const std::type_index COMPLETED = typeid(SkillPhase::Completed);
	inline const std::type_index INTERRUPTED = typeid(SkillPhase::Interrupted);
}