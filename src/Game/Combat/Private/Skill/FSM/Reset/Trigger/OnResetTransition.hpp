#pragma once


#include "IResetTrigger.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Private/Skill/Def/SkillDef.hpp"

#include <optional>

namespace Game::Combat::Skill::FSM::Reset
{


	struct OnResetTransition : IResetTrigger
	{
		std::optional<std::type_index> from;
		std::type_index to;

		OnResetTransition(std::optional<std::type_index> from, std::type_index to)
			: from(from), to(to) { }

		bool evaluate(
			const Game::Combat::Skill::FSM::SkillFSMContext& ctx,
			const Game::Combat::Skill::Def::SkillDef& def,
			std::type_index current,
			std::type_index previous
		) const override
		{
			// std::nullptr => any state 
			if (from.has_value() && previous != from.value()) return false;// fromが有効かつ 以前の状態がfromの値と同じであれば評価する fromがnulloptならpreviousを無視する
			return current == to;
		}
	};
}