#pragma once

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include <typeindex>

namespace Game::Combat::Skill::FSM::Effect
{
	using namespace Game::Combat::Skill::Def;
	using namespace Game::Combat::Skill::FSM;

	struct IEffectTriggerCondition
	{
		virtual ~IEffectTriggerCondition() = default;

		virtual bool evaluate(
			const SkillFSMContext& ctx,
			const SkillDef& def,
			const std::type_index& currentState,
			const std::type_index& previousState
		) const = 0;
	};


}