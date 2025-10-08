#pragma once


namespace Game::Character::Animation::Resolve::Movement
{
	//enum class MoveAnimType : uint8_t
	//{
	//	None,
	//	Idle,
	//	RunFwd,
	//	RunBack,
	//	RunRight,
	//	RunLeft,
	//};

	enum class MoveAnimType
	{
		None,
		Idle,
		RunFwd,RunBack,RunLeft,RunRight,
		RunFwdLeft,RunFwdRight,
		RunBackLeft,RunBackRight
	};

	struct MovementAnimDecisionComponent
	{
		MoveAnimType type = MoveAnimType::None;

		std::string clipKey;
		bool loop = true;
		float playRate = 1.0f;
		bool valid = false;

		// 簡易ヒステリシス用
		// 最適化対象
		MoveAnimType pending = MoveAnimType::None;
		int pendingFrames = 0;
	};
}