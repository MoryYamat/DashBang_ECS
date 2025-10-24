// 状態遷移や滞在により発火する副作用系(副作用=副次動作)

#pragma once



#include "Game/Combat/Private/Skill/FSM/Effect/Template/IEffectTemplate.hpp"
#include "Game/Combat/Private/Skill/FSM/Effect/TriggerCondition/IEffectTriggerCondition.hpp"

#include <typeindex>
#include <vector>
#include <memory>

namespace Game::Combat::Skill::FSM::Effect
{
	// 最適化問題
	struct SkillEffectHook
	{
		std::shared_ptr<IEffectTriggerCondition> trigger;
		std::shared_ptr<IEffectTemplate> effect;
	};
}