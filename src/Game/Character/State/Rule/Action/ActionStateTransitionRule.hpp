#pragma once

#include "Game/Character/State/Component/ActionStateComponent.hpp"

#include <string>
#include <vector>

namespace Game::Character::State::Action
{
	enum class TransitionConditionType
	{ 
		RequestSkillCast,
		RequestRoll,
		RequestGuard,
		IsTimeElapsed,
		// 今後追加
	};

	struct TransitionCondition
	{
		TransitionConditionType type;
		float floatValue = 0.0f;// 時間など
		std::string customData; // スキルIDなど必要に応じて
	};

	struct StateTransitionRule
	{
		ActionState from;
		ActionState to;
		std::vector<TransitionCondition> conditions;
	};
}