#pragma once

#include "Game/Character/Private/FSM/CC/CCStateTags.hpp"

#include <cstdint>
#include <optional>
#include <typeindex>
#include <vector>
#include <algorithm>

namespace Game::Character::FSM::CC::StateModel
{


	struct CCFSMTransitionRequest
	{
		std::optional<std::type_index> requestedTo = StateTag::NONE;
		uint32_t priority = 0;
		std::optional<std::uint32_t> causeId;// eventId
	};

	struct CCFSMTransitionRequestComponent
	{
		std::vector<CCFSMTransitionRequest> requests;

		bool hasExactRequest(const std::type_index& target, uint32_t priority) const
		{
			// いずれか
			return std::any_of(requests.begin(), requests.end(),
				[&](const CCFSMTransitionRequest& req)
				{
					return req.requestedTo == target && req.priority == priority;
				});
		};
	};
}