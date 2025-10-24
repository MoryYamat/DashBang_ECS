#pragma once


namespace Game::Character::State::CC
{
	// 状態異常などの受動的妨害
	enum class CCState
	{
		None,
		Stunned, // スタン
		KnockedBack // ノックバック
	};

	struct CharacterCrowdControlStateComponent
	{
		CCState current = CCState::None;
		CCState previous = CCState::None;
	};
}