#pragma once
#include "Game/Character/State/Component/MovementStateComponent.hpp"

#include "Game/Character/State/Rule/Movement/MovementStateTranstionRule.hpp"

#include "Common/GameNamespaceDecl.h"

#include <vector>

namespace Game::Character::State::Movement
{
	struct StateTransitionDatabase
	{
		std::vector<gNsCharaMoveState::StateTransitionRule> rules;

		const gNsCharaMoveState::StateTransitionRule* FindRule(gNsCharaMoveState::MovementState from, gNsCharaMoveState::MovementState to) const
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