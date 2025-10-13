#pragma once

#include "Game/Character/FSM/CC/CCStateTags.hpp"

#include <cstdint>
#include <optional>
#include <typeindex>

namespace Game::Character::Animation::Query::CC
{
	struct CCAnimationQueryComponent
	{
		bool isCCActive = false;
		std::type_index current = Game::Character::FSM::CC::StateTag::NONE;

		float appliedAt = 0.0f;

		// std::optional<uint32_t> causeId = 0;

		bool valid = false;
	};
}