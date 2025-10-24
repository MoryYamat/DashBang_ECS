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

	inline const char* ToString(MovementState state)
	{
		switch (state)
		{
		case Game::Character::State::Movement::MovementState::Idle:
			return "Idle";
			break;
		case Game::Character::State::Movement::MovementState::Moving:
			return "Moving";
			break;
		default:
			return "Unknown";
			break;
		}
	}
}