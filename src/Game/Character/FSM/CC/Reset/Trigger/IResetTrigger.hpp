#pragma once

#include <optional>
#include <typeindex>

namespace Game::Character::FSM::CC::Reset
{
	struct IResetTrigger
	{
		virtual ~IResetTrigger() = default;

		virtual bool evaluate(
			std::type_index current,
			std::type_index previous
		) const = 0;
	};
}