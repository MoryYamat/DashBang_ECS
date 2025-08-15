#pragma once


#include "Game/Character/FSM/Movement/StateModel/MovementFSMStates.hpp"

#include <typeindex>

namespace Game::Character::FSM::Movement::AxisTag
{
	using namespace Game::Character::FSM::Movement;
	inline const std::type_index MovementAxis = typeid(MovementAxis);
}

namespace Game::Character::FSM::Movement::StateTag
{
	using namespace Game::Character::FSM::Movement;

	inline const std::type_index IDLE = typeid(MovementState::Idle);
	inline const std::type_index MOVING = typeid(MovementState::Moving);
}