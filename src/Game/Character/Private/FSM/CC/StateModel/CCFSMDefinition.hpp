#pragma once

#include "Game/Character/Private/FSM/CC/StateModel/CCFSMConditions.hpp"

#include "Game/Character/Private/FSM/CC/StateModel/CCTransitionTable.hpp"

#include "Game/Character/Private/FSM/CC/Effect/StateScoped/Hook/CCFSMStateEffectHook.hpp"

// reset
#include "Game/Character/Private/FSM/CC/Reset/ResetHookDefinition.hpp"


#include "Game/Character/Private/FSM/CC/CCStateTags.hpp"

#include <vector>
#include <typeindex>

namespace Game::Character::FSM::CC::StateModel
{
	
	struct CCFSMDefinition
	{
		std::type_index initialState = StateTag::NONE;
		std::vector<Game::Character::FSM::CC::StateModel::CCTransition> transitions;
		std::vector<Game::Character::FSM::CC::StateEffect::CCFSMStateEffectHook> hooks;// 副作用
		std::vector<Game::Character::FSM::CC::Reset::CCFSMResetHook> resetHooks;
	};
}