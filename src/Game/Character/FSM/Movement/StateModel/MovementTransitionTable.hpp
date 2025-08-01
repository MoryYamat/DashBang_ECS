#pragma once


#include "Game/Character/FSM/Movement/Effect/StateScoped/Hook/MovementEffectHook.hpp"
#include "Game/Character/FSM/Movement/StateModel/MovementConditions.hpp"

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