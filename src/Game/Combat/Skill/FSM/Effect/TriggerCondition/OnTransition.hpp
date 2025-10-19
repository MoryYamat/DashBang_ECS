// 状態遷移で発火

#pragma once

#include "IEffectTriggerCondition.hpp"

#include <optional>

#include <typeindex>

namespace Game::Combat::Skill::FSM::Effect
{
	
	struct OnTransition : IEffectTriggerCondition
	{
		std::optional<std::type_index> from;
		std::type_index to;

		explicit OnTransition(std::optional<std::type_index> from, std::type_index to)
			: from(from), to(to) { }

		bool evaluate(const SkillFSMContext& ctx,
			const Game::Combat::Skill::Def::SkillDef& def,
			const std::type_index& currentState,
			const std::type_index& previousState) const override
		{
			// std::nullptr => any state 
			if (from.has_value() && previousState != from.value()) return false; 
			return currentState == to;
		}
	};
}