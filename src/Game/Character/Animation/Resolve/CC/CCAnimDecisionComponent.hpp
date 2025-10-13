#pragma once

#include <string>

namespace Game::Character::Animation::Resolve::CC
{
	struct CCAnimDecisionComponent
	{
		std::string clipKey;
		bool loop = false;
		float playRate = 1.0f;
		bool valid = false;
	};
}