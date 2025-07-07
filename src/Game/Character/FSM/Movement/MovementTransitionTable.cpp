#include "MovementTransitionTable.hpp"

std::vector<Game::Character::FSM::Movement::MovementTransition> Game::Character::FSM::Movement::BuildMovementTransitionTable()
{
	using namespace Game::Character::FSM::Movement;
	using namespace Game::Character::FSM::Movement::MovementState;

	return 
	{
		{typeid(Idle), typeid(Moving), std::make_shared<CanMove>()},
		{typeid(Moving), typeid(Idle), std::make_shared<ShouldStop>()}
	};
}