#pragma once

#include "Game/Character/FSM/CC/StateModel/CCFSMStates.hpp"

#include <typeindex>

namespace Game::Character::FSM::CC::AxisTag
{
	using namespace Game::Character::FSM::CC::StateModel;
	inline const std::type_index CCAxis = typeid(CCAxis);
}

namespace Game::Character::FSM::CC::StateTag
{
	using namespace Game::Character::FSM::CC::StateModel;

	inline const std::type_index NONE = typeid(CCState::None);
	inline const std::type_index STUNNED = typeid(CCState::Stunned);
	inline const std::type_index KNOCKDOWNED = typeid(CCState::Knockdowned);
}