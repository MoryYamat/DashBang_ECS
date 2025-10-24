#pragma once


#include "Game/Character/Private/FSM/Movement/Effect/StateScoped/TriggerCondition/IEffectTriggerCondition.hpp"

#include <typeindex>

namespace Game::Character::FSM::Movement::Effect
{
	struct WhileInState : IEffectTriggerCondition
	{
		std::type_index state;

		explicit WhileInState(std::type_index s)
			: state(s) { }

		bool evaluate(const MovementFSMContext& ctx,
			const std::type_index& currentState,
			const std::type_index& previousState) const override
		{
			return currentState == state;
		}
	};
}