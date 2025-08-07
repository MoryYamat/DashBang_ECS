#pragma once

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"

namespace Game::Combat::Skill::FSM::Reset
{
	using namespace Game::Combat::Skill::FSM;

	using namespace Game::Combat::Skill::Def;

	struct IResetTrigger
	{
		virtual ~IResetTrigger() = default;

		virtual bool evaluate(
			const SkillFSMContext& ctx,
			const SkillDef& def,
			std::type_index current,
			std::type_index previous
		) const = 0;
	};
}