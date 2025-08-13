#pragma once

#include "Game/Character/FSM/CC/StateModel/CCFSMConditions.hpp"

#include "Game/Character/FSM/CC/StateModel/CCTransitionTable.hpp"

#include "Game/Character/FSM/CC/CCStateTags.hpp"

#include <vector>
#include <typeindex>

namespace Game::Character::FSM::CC::StateModel
{
	using namespace Game::Character::FSM::CC;

	struct CCFSMDefinition
	{
		std::type_index initialState = StateTag::NONE;
		std::vector<CCTransition> transitions;
	};
}