#pragma once

#include "Game/Character/Private/FSM/CC/StateModel/CCFSMContext.hpp"

#include <optional>
#include <typeindex>

namespace Game::Character::FSM::CC::StateEffect
{

	struct IStateEffectTriggerCondition
	{
		virtual ~IStateEffectTriggerCondition() = default;


		virtual bool evaluate(
			const Game::Character::FSM::CC::StateModel::CCFSMContext& ctx,
			const std::type_index& currentState,
			const std::type_index& previousState
		) const = 0;
	};

	struct OnTransition : IStateEffectTriggerCondition
	{
		std::optional<std::type_index> from;
		std::type_index to;

		explicit OnTransition(std::optional<std::type_index> from, std::type_index to)
			: from(from), to(to) {
		}

		bool evaluate(
			const Game::Character::FSM::CC::StateModel::CCFSMContext& ctx,
			const std::type_index& currentState,
			const std::type_index& previousState
		) const override
		{
			// std::nullptr => any state 
			if (from.has_value() && previousState != from.value()) return false;
			return currentState == to;
		}
	};
}