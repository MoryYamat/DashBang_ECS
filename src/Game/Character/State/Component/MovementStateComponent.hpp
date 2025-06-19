#pragma once


namespace Game::Character::State::Movement
{
	enum class MovementState
	{
		Idle,
		Moving,
	};

	struct MovementStateComponent
	{
		MovementState current = MovementState::Idle;
		// MovementState previous = MovementState::Idle;
	};
}