#pragma once

#include "Game/Character/FSM/CC/StateModel/CCFSMContext.hpp"

#include <typeindex>

namespace Game::Character::FSM::CC::StateEffect
{

	struct IStateEffectTriggerCondition
	{
		virtual ~IStateEffectTriggerCondition() = default;


		virtual bool evaluate(
			const CCFSMContext& ctx,
			const std::type_index& currentState,
			const std::type_index& previousState
		) const = 0;
	};
}