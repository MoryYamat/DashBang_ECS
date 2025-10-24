#pragma once

#include "Game/Character/Private/FSM/Movement/MovementStateTags.hpp"

#include <optional>
#include <typeindex>
#include <vector>
#include <cstdint>
#include <algorithm>

// TODO: 副作用としての移動
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
			// いずれか
			return std::any_of(requests.begin(), requests.end(),
				[&](const MovementFSMTransitionRequest& req)
				{
					return req.requestedTo == target && req.priority == priority;
				});
		};
	};
}