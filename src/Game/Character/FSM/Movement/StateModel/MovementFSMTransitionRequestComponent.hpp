#pragma once

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

#include <optional>
#include <typeindex>
#include <vector>
#include <cstdint>
#include <algorithm>

// TODO: •›ì—p‚Æ‚µ‚Ä‚ÌˆÚ“®
namespace Game::Character::FSM::Movement
{

	struct MovementFSMTransitionRequest
	{
		std::type_index requestedTo = StateTag::IDLE;
		uint32_t priority = 0;
	};

	struct MovementFSMTransitionRequestComponent
	{
		std::vector<MovementFSMTransitionRequest> requests;

		bool hasExactRequest(const std::type_index& target, uint32_t priority) const
		{
			// ‚¢‚¸‚ê‚©
			return std::any_of(requests.begin(), requests.end(),
				[&](const MovementFSMTransitionRequest& req)
				{
					return req.requestedTo == target && req.priority == priority;
				});
		};
	};
}