#pragma once

#include "Game/Combat/Skill/FSM/Reset/Handler/IResetHandler.hpp"
#include "Game/Combat/Skill/FSM/Reset/Trigger/IResetTrigger.hpp"

#include <typeindex>
#include <memory>
#include <vector>
namespace Game::Combat::Skill::FSM::Reset
{
	struct SkillFSMResetHook
	{
		std::vector<std::shared_ptr<IResetHandler>> handlers;
		std::shared_ptr<IResetTrigger> trigger;
	};
}