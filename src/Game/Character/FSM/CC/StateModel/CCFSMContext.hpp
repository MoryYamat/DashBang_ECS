#pragma once

#include "Game/Character/FSM/CC/CCStateTags.hpp"

#include <typeindex>

namespace Game::Character::FSM::CC::StateModel
{

	struct CCFSMContext
	{
		std::type_index dominantTag = StateTag::NONE;
		bool hasActiveCC = false;
	};
}