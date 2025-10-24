#pragma once

#include "Game/Character/Private/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"

#include <vector>

namespace Game::Combat::Skill::FSM::StateModel
{
	struct SkillFSMInterferenceRequestComponent
	{
		std::vector<Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest> requests;
	};
}