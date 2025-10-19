#pragma once

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include <typeindex>

namespace Game::Combat::Skill::FSM::Effect
{

	struct IEffectTriggerCondition
	{
		virtual ~IEffectTriggerCondition() = default;

		virtual bool evaluate(
			const Game::Combat::Skill::FSM::SkillFSMContext& ctx,
			const Game::Combat::Skill::Def::SkillDef& def,
			const std::type_index& currentState,
			const std::type_index& previousState
		) const = 0;
	};


}