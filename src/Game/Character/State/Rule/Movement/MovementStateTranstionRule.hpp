#pragma once

#include "Game/Character/State/Component/MovementStateComponent.hpp"

#include <string>
#include <vector>

namespace Game::Character::State::Movement
{
	enum class TransitionConditionType
	{
		IsActualMoving,// 論理移動量が一定以上(移動中)
		IsStationaly,// 論理移動量が一定未満 (停止)
		TimeElapsed// 状態滞在時間など(補助条件)
	};

	struct TransitionCondition
	{
		TransitionConditionType type;
		float floatValue = 0.0f;// 速度閾値，経過時間など
		std::string customData;
	};

	struct StateTransitionRule
	{
		MovementState from;
		MovementState to;
		std::vector<TransitionCondition> conditions;
	};
}