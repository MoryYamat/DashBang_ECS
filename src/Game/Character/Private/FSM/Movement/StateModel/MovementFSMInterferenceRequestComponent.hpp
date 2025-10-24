#pragma once

#include "Game/Character/Private/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"


namespace Game::Character::FSM::Movement::StateModel
{
	struct MovementFSMInterferenceRequestComponent
	{
		std::vector<Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest> requests;

		bool hasActiveRequest()
		{
			for (const auto& req : requests)
			{
				if (req.durationSec > 0.0f)
				{
					return true;
				}
			}
			return false;
		}
	};
}