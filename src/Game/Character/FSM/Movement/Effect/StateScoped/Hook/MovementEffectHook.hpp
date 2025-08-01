// ó‘Ô‘JˆÚ‚â‘Øİ‚É‚æ‚è”­‰Î‚·‚é•›ì—pŒn(•›ì—p=•›Ÿ“®ì)

#pragma once

#include "Game/Character/FSM/Movement/Effect/StateScoped/TriggerCondition/IEffectTriggerCondition.hpp"
#include "Game/Character/FSM/Movement/Effect/StateScoped/Template/IEffectTemplate.hpp"

#include <typeindex>
#include <vector>
#include <memory>

namespace Game::Character::FSM::Movement::Effect
{
	// Å“K‰»–â‘è
	struct MovementEffectHook
	{
		std::shared_ptr<IEffectTriggerCondition> trigger;
		std::shared_ptr<IEffectTemplate> effect;
	};
}