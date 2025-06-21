#include "ActionStateTransitionEvaluator.hpp"


bool Game::Character::State::Action::AreConditionsMet
(
	const gNsCharaActionState::CharacterActionStateComponent& action,
	const gNsSkillIntent::SkillIntentComponent& intent,
	float deltaTime,
	const std::vector<gNsCharaActionState::TransitionCondition>& conditions
)
{
	for (const auto& cond : conditions)
	{
		if (!EvaluateCondition(action, intent, deltaTime, cond))
			return false;
	}
	return true;
}


bool Game::Character::State::Action::EvaluateCondition
(
	const gNsCharaActionState::CharacterActionStateComponent& action,
	const gNsSkillIntent::SkillIntentComponent& intent,
	float deltaTime,
	const gNsCharaActionState::TransitionCondition& condition
)
{
	using Condition = gNsCharaActionState::TransitionConditionType;

	switch (condition.type)
	{
	case Condition::RequestSkillCast:
		// 仮のフラグを current 状態から判定
		// return intent.isActive && !intent.requestedSlots.empty();
		return intent.isActive;

	//case Condition::RequestRoll:
	//	// 同様に None 状態なら回避可能（入力など未実装の暫定対応）
	//	return action.current == gNsCharaActionState::ActionState::None;

	//case Condition::RequestGuard:
	//	return action.current == gNsCharaActionState::ActionState::None;

	case Condition::IsTimeElapsed:
	{
		static float elapsedTime = 0.0f;
		elapsedTime += deltaTime;

		if (elapsedTime >= condition.floatValue) {
			elapsedTime = 0.0f; // 条件を満たしたらリセット
			return true;
		}
		return false;
	}

	default:
		return false;
	}

}