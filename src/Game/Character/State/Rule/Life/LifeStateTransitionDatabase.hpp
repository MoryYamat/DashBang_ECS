#pragma once

#include "Game/Character/State/Component/LifeStateComponent.hpp"

#include "Game/Character/State/Rule/Life/LifeStateTransitionRule.hpp"

#include "Common/GameNamespaceDecl.h"

#include <vector>
namespace Game::Character::State::Life
{
	struct StateTransitionDatabase
	{
		std::vector<gNsCharaLifeState::StateTransitionRule> rules;

		const StateTransitionRule* FindRule(LifeState from, LifeState to) const
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