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

// Fixme: ここで条件評価の定義をしているのはおかしい気がする
// Fixme: ここで条件評価の定義をしているのはおかしい気がする
// Fixme: ここで条件評価の定義をしているのはおかしい気がする
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
	case Condition::SkillTriggered:
		// 仮のフラグを current 状態から判定
		// return intent.isActive && !intent.requestedSlots.empty();
		return intent.isActive;// 仮のフラグ
		// 本当は，キャラクターのスキル実行状態に基づいている必要がある
		// このRequestSkillCastは，トリガーがおされて，スキルが実行可能な場合遷移する

	case Condition::ActionDurationElapsed:
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