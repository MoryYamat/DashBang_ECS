#include "LifeStateTransitionEvaluator.hpp"
 
#include "Engine/Debug/DebugUtils.h"

#include <iostream>

bool Game::Character::State::Life::AreConditionsMet
(
	const gNsCharaLifeState::CharacterLifeStateComponent& lifeComp,// 現在未使用 current / previous を使う場合は必要
	const gNsCharacterStats::CharacterStatsComponent& statsComp,
	const std::vector<gNsCharaLifeState::TransitionCondition>& conditions
)
{
	for (const auto& cond : conditions)
	{
		if (!EvaluateCondition(lifeComp, statsComp, cond))
			return false;
	}
	return true;
}

bool Game::Character::State::Life::EvaluateCondition
(
	const gNsCharaLifeState::CharacterLifeStateComponent& lifeComp,// 現在未使用 current / previous を使う場合は必要
	const gNsCharacterStats::CharacterStatsComponent& statsComp,
	const gNsCharaLifeState::TransitionCondition& condition
)
{
	// DEBUG_LOG("here");

	// ここのハードコードは最適化できないか？：ディスパッチャの実装など
	// ここのハードコードは最適化できないか？：ディスパッチャの実装など
	// ここのハードコードは最適化できないか？：ディスパッチャの実装など
	switch (condition.type)
	{
	case TransitionConditionType::HPIsZeroOrLess:
		return statsComp.currentHP <= 0.0f;

	case TransitionConditionType::ExplicitReviveFlag:
		// 明示的に何か外部の revive フラグが立ってる場合（未実装なら常に false）
		return false;

	default:
		return false;
	}
}