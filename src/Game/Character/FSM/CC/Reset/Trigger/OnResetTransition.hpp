#pragma once

#include "IResetTrigger.hpp"


#include <optional>
#include <typeindex>
namespace Game::Character::FSM::CC::Reset
{
	struct OnResetTransition : IResetTrigger
	{
		std::optional<std::type_index> from;
		std::type_index to;

		OnResetTransition(std::optional<std::type_index> from, std::type_index to) :
			from(from), to(to) { }

		bool evaluate
		(
			std::type_index current,
			std::type_index previous
		)const override
		{
			// std::nullptr => any state 
			// fromが有効かつ 以前の状態がfromの値と同じであれば評価する fromがnulloptならpreviousを無視する
			if (from.has_value() && previous != from.value()) return false;
			return current == to;
		}

	};
}