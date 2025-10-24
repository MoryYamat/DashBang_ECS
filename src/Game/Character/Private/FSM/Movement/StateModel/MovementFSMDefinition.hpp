#pragma once

#include "Game/Character/Private/FSM/Movement/MovementStateTags.hpp"

#include "Game/Character/Private/FSM/Movement/StateModel/MovementTransitionTable.hpp"
#include "Game/Character/Private/FSM/Movement/Effect/StateScoped/Hook/MovementEffectHook.hpp"

#include "Game/Character/Private/FSM/Movement/Effect/FSMScoped/IFSMScopedEffect.hpp"



#include <vector>
#include <typeindex>


namespace Game::Character::FSM::Movement
{
	struct MovementFSMDefinition
	{
		std::type_index initialState = typeid(StateTag::IDLE);
		std::vector<MovementTransition> transitions;
		std::vector<Game::Character::FSM::Movement::Effect::MovementEffectHook> effectHooks;// stateScoped Effect
		std::vector<std::shared_ptr<Game::Character::FSM::Movement::Effect::FSMScoped::IFSMScopedEffect>> fsmScopedEffects;// fsm scoped effect
	};
}