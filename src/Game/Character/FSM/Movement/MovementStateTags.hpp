#pragma once

#include "Game/Character/FSM/Movement/StateModel/MovementFSMStates.hpp"

#include <typeindex>

namespace Game::Character::FSM::Movement::AxisTag
{
	inline const std::type_index MovementAxis = typeid(Game::Character::FSM::Movement::MovementAxis);
}

namespace Game::Character::FSM::Movement::StateTag
{

	inline const std::type_index IDLE = typeid(Game::Character::FSM::Movement::MovementState::Idle);
	inline const std::type_index MOVING = typeid(Game::Character::FSM::Movement::MovementState::Moving);
}