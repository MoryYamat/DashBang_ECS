#pragma once


namespace Game::Character::Animation::Resolve::Movement
{
	enum class MoveAnimType : uint8_t
	{
		None,
		Idle,
		RunFwd,
		RunBack,
		RunRight,
		RunLeft,
	};

	struct MovementAnimDecisionComponent
	{
		MoveAnimType type = MoveAnimType::None;

		std::string clipKey;
		bool loop = true;
		float playRate = 1.0f;
		bool valid = false;
	};
}