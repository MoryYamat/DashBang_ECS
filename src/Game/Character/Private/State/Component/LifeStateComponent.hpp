#pragma once


namespace Game::Character::State::Life
{
	enum class LifeState
	{
		Alive,
		Dead

		
	};

	struct CharacterLifeStateComponent
	{
		LifeState current = LifeState::Alive;
		// LifeState previous = LifeState::Alive;
	};

	inline const char* ToString(LifeState state)
	{
		switch (state)
		{
		case Game::Character::State::Life::LifeState::Alive:
			return "Alive";
			break;
		case Game::Character::State::Life::LifeState::Dead:
			return "Dead";
			break;
		default:
			return "Unknown";
			break;
		}
	}
}
