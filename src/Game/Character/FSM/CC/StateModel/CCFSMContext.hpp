#pragma once



#include <typeindex>

namespace Game::Character::FSM::CC::StateModel
{
	struct CCFSMContext
	{
		std::type_index dominantTag;
		bool hasActiveCC = false;
	};
}