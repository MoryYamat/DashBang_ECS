#pragma once

#include "IEffectTriggerCondition.hpp"

// Œo‰ßŽžŠÔ‚Å”­‰Î

#include <typeindex>

namespace  Game::Combat::Skill::FSM::Effect
{
	struct OnStateElapsedTime : IEffectTriggerCondition
	{
		std::type_index state;
		float threshold;

		explicit OnStateElapsedTime(std::type_index s, float t)
			: state(s), threshold(t) { }

		bool evaluate(const SkillFSMContext& ctx,
			const SkillDef& def,
			const std::type_index& currentState,
			const std::type_index& previousState
		) const override
		{
			return currentState == state && ctx.phaseElapsedTime >= threshold;
		}
	};
}