#pragma once
#include "Game/Character/State/Component/MovementStateComponent.hpp"

#include "Game/Character/State/Rule/Movement/MovementStateTranstionRule.hpp"



#include <vector>

namespace Game::Character::State::Movement
{
	struct StateTransitionDatabase
	{
		std::vector<Game::Character::State::Movement::StateTransitionRule> rules;

		const Game::Character::State::Movement::StateTransitionRule* FindRule(Game::Character::State::Movement::MovementState from, Game::Character::State::Movement::MovementState to) const
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