#pragma once

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

#include "Game/Character/FSM/Movement/StateModel/MovementTransitionTable.hpp"
#include "Game/Character/FSM/Movement/Effect/StateScoped/Hook/MovementEffectHook.hpp"

#include "Game/Character/FSM/Movement/Effect/FSMScoped/IFSMScopedEffect.hpp"

#include "Common/GameNamespaceDecl.h"

#include <vector>
#include <typeindex>


namespace Game::Character::FSM::Movement
{
	using namespace Game::Character::FSM::Movement::Effect;
	using namespace Game::Character::FSM::Movement::Effect::FSMScoped;
	struct MovementFSMDefinition
	{
		std::type_index initialState = StateTag::IDLE;
		std::vector<MovementTransition> transitions;
		std::vector<MovementEffectHook> effectHooks;// stateScoped Effect
		std::vector<std::shared_ptr<IFSMScopedEffect>> fsmScopedEffects;// fsm scoped effect
	};
}