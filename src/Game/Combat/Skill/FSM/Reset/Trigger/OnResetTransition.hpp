#pragma once


#include "IResetTrigger.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include <optional>

namespace Game::Combat::Skill::FSM::Reset
{
	using namespace Game::Combat::Skill::FSM;

	using namespace Game::Combat::Skill::Def;

	struct OnResetTransition : IResetTrigger
	{
		std::optional<std::type_index> from;
		std::type_index to;

		OnResetTransition(std::optional<std::type_index> from, std::type_index to)
			: from(from), to(to) { }

		bool evaluate(
			const SkillFSMContext& ctx,
			const SkillDef& def,
			std::type_index current,
			std::type_index previous
		) const override
		{
			// std::nullptr => any state 
			if (from.has_value() && previous != from.value()) return false;// from‚ª—LŒø‚©‚Â ˆÈ‘O‚Ìó‘Ô‚ªfrom‚Ì’l‚Æ“¯‚¶‚Å‚ ‚ê‚Î•]‰¿‚·‚é from‚ªnullopt‚È‚çprevious‚ğ–³‹‚·‚é
			return current == to;
		}
	};
}