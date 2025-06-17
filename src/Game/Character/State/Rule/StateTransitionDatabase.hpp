//
#pragma once

#include "Game/Character/State/Behavior/CharacterState.h"

#include "Game/Character/State/Rule/StateTransitionRule.hpp"

#include <vector>

#include <Common/GameNamespaceDecl.h>

namespace Game::Character::State
{
	struct StateTransitionDatabase
	{
		std::vector<StateTransitionRule> rules;

		const StateTransitionRule* FindRule(
			CharacterBehaviorState from,
			CharacterBehaviorState to
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