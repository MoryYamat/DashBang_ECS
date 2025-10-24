#pragma once

#include <string>

namespace Game::Character::Animation::Arbiter
{
	struct FinalAnimationDecisionComponent
	{
		bool valid = false;
		std::string clipKey;
		bool loop = true;
		float playRate = 1.0f;

		// 将来は重みや優先度などを増やす

	};
}