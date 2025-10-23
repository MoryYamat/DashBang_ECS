#include "MovementStateTransitionEvaluator.hpp"

#include "Engine/Math/Private/Logic2D/LogicMathUtils.h"

// 遷移条件をすべて評価
bool Game::Character::State::Movement::AreConditionsMet
(
	const Game::Character::State::Movement::CharacterMovementStateComponent& movement,// 現在未使用 current / previous を使う場合は必要
	const Engine::ECS::Component::Logic2D::Velocity2DComponent& vel,
	float deltaTime,
	const std::vector<Game::Character::State::Movement::TransitionCondition>& conditions
)
{
	for (const auto& cond : conditions)
	{
		if (!EvaluateCondition(vel, deltaTime, cond))
			return false;
	}
	return true;
}

// 遷移条件の単体評価
bool Game::Character::State::Movement::EvaluateCondition
(
	const Engine::ECS::Component::Logic2D::Velocity2DComponent& vel,
	float deltaTime,
	const Game::Character::State::Movement::TransitionCondition& cond
)
{
	const float vel2 = Engine::Math::Logic2D::Vector::SquaredLength(vel.velocity);

	// ここのハードコードは最適化できないか？：ディスパッチャの実装など
	// ここのハードコードは最適化できないか？：ディスパッチャの実装など
	// ここのハードコードは最適化できないか？：ディスパッチャの実装など
	switch (cond.type)
	{
	case TransitionConditionType::IsActualMoving:
		return vel2 > cond.floatValue * cond.floatValue;
	case TransitionConditionType::IsStationaly:
		return vel2 <= cond.floatValue * cond.floatValue;
	case TransitionConditionType::TimeElapsed:
		return deltaTime > cond.floatValue;
	default:
		return false;
	}
}