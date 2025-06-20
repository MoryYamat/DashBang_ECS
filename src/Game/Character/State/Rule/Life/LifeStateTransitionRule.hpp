#pragma once

#include "Game/Character/State/Component/LifeStateComponent.hpp"

#include <string>

#include <vector>

namespace Game::Character::State::Life
{
	enum class TransitionConditionType
	{
		HPIsZeroOrLess,
		ExplicitReviveFlag// 明示的な蘇生(イベント)
	};

	struct TransitionCondition
	{
		TransitionConditionType type;
		float floatValue = 0.0f;
		std::string customData;
	};

	struct StateTransitionRule
	{
		LifeState from;
		LifeState to;
		std::vector<TransitionCondition> conditions;
	};
}