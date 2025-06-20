#pragma once


namespace Game::Character::State::Movement
{
	enum class MovementState
	{
		Idle,
		Moving,
	};

	struct CharacterMovementStateComponent
	{
		MovementState current = MovementState::Idle;
		// MovementState previous = MovementState::Idle;
	};
}