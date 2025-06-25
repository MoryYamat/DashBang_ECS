#include "ActionStateTransitionEvaluator.hpp"


bool Game::Character::State::Action::AreConditionsMet
(
	const gNsCharaActionState::CharacterActionStateComponent& action,
	const gNsCharaActionState::CharacterSkillExecutionStateComponent& skillExec,
	float deltaTime,
	const std::vector<gNsCharaActionState::TransitionCondition>& conditions
)
{
	for (const auto& cond : conditions)
	{
		if (!EvaluateCondition(action, skillExec, deltaTime, cond))
			return false;
	}
	return true;
}

bool Game::Character::State::Action::EvaluateCondition
(
	const gNsCharaActionState::CharacterActionStateComponent& action,
	const gNsCharaActionState::CharacterSkillExecutionStateComponent& skillExec,
	float deltaTime,
	const gNsCharaActionState::TransitionCondition& condition
)
{
	using Condition = gNsCharaActionState::TransitionConditionType;
	switch (condition.type)
	{
	case Condition::SkillTriggered:
		// キャラクターのスキル実行状態が開始されたかどうか
		return skillExec.previousPhase == gNsCharaActionState::CharacterSkillExecutionPhase::None
			&& skillExec.currentPhase == gNsCharaActionState::CharacterSkillExecutionPhase::Casting;
		// Fixme: ActionDurationElapsedという名前はあいまいで誤解が生じやすい：例えば，SkillExitTriggeredのように，スキルが終了するトリガーが発生したことがわかるような名前がよい
	case Condition::ActionDurationElapsed:
		// キャラクターのスキル実行状態が完了したか，スキル実行が中断された場合
		return skillExec.currentPhase == Character::State::Action::CharacterSkillExecutionPhase::Completed
			|| skillExec.currentPhase == Character::State::Action::CharacterSkillExecutionPhase::Interrupted;
	default:
		return false;
	}
}
