#include "StateTransitionEvaluator.hpp"


bool Game::Character::State::StateTransitionEvaluator::AreConditionsMet
(
	const CharacterStateComponent& stateComp,
	float deltaTime,
	const std::vector<TransitionCondition>& conditions
)
{
	for (const auto& condition : conditions)
	{
		if (!EvaluateCondition(stateComp, deltaTime, condition))
		{
			// 拡張案：任意で「OR条件」や「条件グループ」などにも対応可能
			// 例：少なくとも1つ通ればOKとか

			return false;// 一つでも満たさなければNG (AND)
		}
	}

	return true;
}


// 各条件を個別に評価するヘルパー
bool Game::Character::State::StateTransitionEvaluator::EvaluateCondition(
	const CharacterStateComponent& stateComp,
	float deltaTime,
	const TransitionCondition& condition
)
{
	switch (condition.type)
	{
	case TransitionConditionType::CanBeInterrupted:
		return stateComp.canBeInterrupted;

	case TransitionConditionType::NotCrowdControlled:
		return stateComp.crowdControl == CharacterCrowdControlState::None;

	case TransitionConditionType::HasTimeElapsed:
		return stateComp.timeInState >= condition.floatValue;

		// 今後：HP, AIのcontext, 位置 などを追加可能
	default:
		return false;
	}
}