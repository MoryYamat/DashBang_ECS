// ó‘Ô‘JˆÚ‚â‘Øİ‚É‚æ‚è”­‰Î‚·‚é•›ì—pŒn(•›ì—p=•›Ÿ“®ì)

#pragma once



#include "Game/Combat/Skill/FSM/Effect/Template/IEffectTemplate.hpp"
#include "Game/Combat/Skill/FSM/Effect/TriggerCondition/IEffectTriggerCondition.hpp"

#include <typeindex>
#include <vector>
#include <memory>

namespace Game::Combat::Skill::FSM::Effect
{
	struct SkillEffectHook
	{
		std::shared_ptr<IEffectTriggerConditrion> trigger;
		std::shared_ptr<IEffectTemplate> effect;
	};
}