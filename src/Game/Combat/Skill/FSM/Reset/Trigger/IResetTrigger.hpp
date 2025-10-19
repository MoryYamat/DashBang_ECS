#pragma once

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"

namespace Game::Combat::Skill::FSM::Reset
{


	struct IResetTrigger
	{
		virtual ~IResetTrigger() = default;

		virtual bool evaluate(
			const Game::Combat::Skill::FSM::SkillFSMContext& ctx,
			const Game::Combat::Skill::Def::SkillDef& def,
			std::type_index current,
			std::type_index previous
		) const = 0;
	};
}