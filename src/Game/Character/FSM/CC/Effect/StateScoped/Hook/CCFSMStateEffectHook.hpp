#pragma once

#include "Game/Character/FSM/CC/Effect/StateScoped/Handler/IStateEffectHandler.hpp"
#include "Game/Character/FSM/CC/Effect/StateScoped/TriggerCondition/IStateEffectTriggerCondition.hpp"
#include <memory>


namespace Game::Character::FSM::CC::StateEffect
{

	struct CCFSMStateEffectHook
	{
		std::shared_ptr<IStateEffectHandler> handler;
		std::shared_ptr<IStateEffectTriggerCondition> trigger;
	};
}