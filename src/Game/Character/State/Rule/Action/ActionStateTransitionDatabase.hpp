#pragma once

#include "Game/Character/State/Rule/Action/ActionStateTransitionRule.hpp"

#include <vector>



namespace Game::Character::State::Action
{
	struct StateTransitionDatabase
	{
		std::vector<Game::Character::State::Action::StateTransitionRule> rules;

		const Game::Character::State::Action::StateTransitionRule* FindRule
		(
			Game::Character::State::Action::ActionState from,
			Game::Character::State::Action::ActionState to
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