#pragma once

#include "Game/Character/FSM/CC/CCStateTags.hpp"

#include <typeindex>
#include <vector>
#include <algorithm>
namespace Game::Character::FSM::CC::StateModel
{


	struct CCFSMTransitionRequest
	{
		std::type_index requestedTo = StateTag::NONE;
		uint32_t priority = 0;
	};

	struct CCFSMTransitionRequestComponent
	{
		std::vector<CCFSMTransitionRequest> requests;

		bool hasExactRequest(const std::type_index& target, uint32_t priority) const
		{
			// ‚¢‚¸‚ê‚©
			return std::any_of(requests.begin(), requests.end(),
				[&](const CCFSMTransitionRequest& req)
				{
					return req.requestedTo == target && req.priority == priority;
				});
		};
	};
}