#include "MovementTransitionTable.hpp"

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

std::vector<Game::Character::FSM::Movement::MovementTransition> Game::Character::FSM::Movement::BuildMovementTransitionTable()
{
	using namespace Game::Character::FSM::Movement;
	

	return 
	{
		{StateTag::IDLE, StateTag::MOVING, std::make_shared<CanMove>()},
		{StateTag::MOVING, StateTag::IDLE, std::make_shared<ShouldStop>()}
	};
}