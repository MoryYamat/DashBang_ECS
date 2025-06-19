#pragma once


namespace Game::Character::State::Life
{
	enum class LifeState
	{
		Alive,
		Dead
	};

	struct LifestateComponent
	{
		LifeState current = LifeState::Alive;
		// LifeState previous = LifeState::Alive;
	};
}
