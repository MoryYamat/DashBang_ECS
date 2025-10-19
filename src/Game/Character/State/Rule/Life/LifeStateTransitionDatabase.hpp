#pragma once

#include "Game/Character/State/Component/LifeStateComponent.hpp"

#include "Game/Character/State/Rule/Life/LifeStateTransitionRule.hpp"



#include <vector>
namespace Game::Character::State::Life
{
	struct StateTransitionDatabase
	{
		std::vector<Game::Character::State::Life::StateTransitionRule> rules;

		const Game::Character::State::Life::StateTransitionRule* FindRule(LifeState from, LifeState to) const
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