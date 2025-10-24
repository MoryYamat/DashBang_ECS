#pragma once

#include "Game/Character/Private/FSM/CC/Reset/Handler/IResetHandler.hpp"
#include "Game/Character/Private/FSM/CC/Reset/Trigger/IResetTrigger.hpp"

#include <vector>
#include <memory>

namespace Game::Character::FSM::CC::Reset
{
	struct CCFSMResetHook
	{
		std::vector<std::shared_ptr<IResetHandler>> handlers;
		std::shared_ptr<IResetTrigger> trigger;
	};
}