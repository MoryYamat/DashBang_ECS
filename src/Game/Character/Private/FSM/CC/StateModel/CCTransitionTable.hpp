#pragma once

#include "Game/Character/Private/FSM/CC/StateModel/CCFSMConditions.hpp"

#include <optional>
#include <typeindex>
#include <memory>
namespace Game::Character::FSM::CC::StateModel
{
	struct CCTransition
	{
		std::optional<std::type_index> from;
		std::type_index to;
		std::shared_ptr<ICCFSMCondition> condition;
		int priority = 0;
	};
}