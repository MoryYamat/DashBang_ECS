#pragma once

#include <string>

namespace Game::Character::Animation::Resolve::Skill
{
	// 
	struct SkillAnimDecisionComponent
	{
		std::string clipKey;
		bool loop = true;
		float playRate = 1.0f;
		bool valid = false;
	};
}