#pragma once

#include "MovementFSMDefinition.hpp"
#include "MovementConditions.hpp"

#include <typeindex>
#include <vector>
#include <memory>

namespace Game::Character::FSM::Movement
{

	struct MovementTransition
	{
		std::type_index from;
		std::type_index to;
		std::shared_ptr<IMovementCondition> condition;
	};

	struct MovementTransitionTableResource
	{
		std::vector<MovementTransition> transitions;
	};

	std::vector<MovementTransition> BuildMovementTransitionTable();


}