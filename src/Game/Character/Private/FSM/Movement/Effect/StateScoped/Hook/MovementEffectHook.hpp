// 状態遷移や滞在により発火する副作用系(副作用=副次動作)

#pragma once

#include "Game/Character/Private/FSM/Movement/Effect/StateScoped/TriggerCondition/IEffectTriggerCondition.hpp"
#include "Game/Character/Private/FSM/Movement/Effect/StateScoped/Template/IEffectTemplate.hpp"

#include <typeindex>
#include <vector>
#include <memory>

namespace Game::Character::FSM::Movement::Effect
{
	// 最適化問題
	struct MovementEffectHook
	{
		std::shared_ptr<IEffectTriggerCondition> trigger;
		std::shared_ptr<IEffectTemplate> effect;
	};
}