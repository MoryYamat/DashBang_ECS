#pragma once

#include "Game/Character/FSM/CC/CCStateTags.hpp"

#include <typeindex>
#include <optional>

namespace Game::Character::FSM::CC::StateModel
{
	// 
	struct CCFSMContext
	{
		
		std::optional<std::type_index> currentCC;
		float ccStart = 0.0f;
		float ccDuration = 0.0f;

		bool immune = false;
		float immuneEndsAt = 0.0f;

		int chainCount = 0;
		float chainWindowStart = 0.0f;
		
		// íœ—\’è
		std::type_index dominantTag = StateTag::NONE;
		bool hasActiveCC = false;
	};
}