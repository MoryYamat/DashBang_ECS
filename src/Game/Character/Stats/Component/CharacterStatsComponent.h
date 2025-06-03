// キャラクターの固有値のデータ群


#pragma once

namespace Game::Character::Stats
{
	struct CharacterStatsComponent
	{
		float moveSpeed = 5.0f;// base move speed
		float acceleration = 0.0f;
	};
}

