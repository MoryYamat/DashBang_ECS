#pragma once

#include "Game/Character/State/Rule/Action/ActionStateTransitionRule.hpp"

#include <vector>

#include "Common/GameNamespaceDecl.h"

namespace Game::Character::State::Action
{
	struct StateTransitionDatabase
	{
		std::vector<gNsCharaActionState::StateTransitionRule> rules;

		const gNsCharaActionState::StateTransitionRule* FindRule
		(
			gNsCharaActionState::ActionState from,
			gNsCharaActionState::ActionState to
		)
		{
			for (const auto& rule : rules)
			{
				if (rule.from == from && rule.to == to)
					return &rule;
			}
			return nullptr;
		}

	};
}