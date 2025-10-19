#include "ActionStateTransitionEvaluator.hpp"


bool Game::Character::State::Action::AreConditionsMet
(
	const Game::Character::State::Action::CharacterActionStateComponent& action,
	const Game::Character::State::Action::CharacterSkillExecutionStateComponent& skillExec,
	float deltaTime,
	const std::vector<Game::Character::State::Action::TransitionCondition>& conditions
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
	const Game::Character::State::Action::CharacterActionStateComponent& action,
	const Game::Character::State::Action::CharacterSkillExecutionStateComponent& skillExec,
	float deltaTime,
	const Game::Character::State::Action::TransitionCondition& condition
)
{
	using Condition = Game::Character::State::Action::TransitionConditionType;
	switch (condition.type)
	{
	case Condition::SkillTriggered:
		// キャラクターのスキル実行状態が開始されたかどうか
		return skillExec.previousPhase == Game::Character::State::Action::CharacterSkillExecutionPhase::None
			&& skillExec.currentPhase == Game::Character::State::Action::CharacterSkillExecutionPhase::Casting;
		// Fixme: ActionDurationElapsedという名前はあいまいで誤解が生じやすい：例えば，SkillExitTriggeredのように，スキルが終了するトリガーが発生したことがわかるような名前がよい
	case Condition::ActionDurationElapsed:
		// キャラクターのスキル実行状態が完了したか，スキル実行が中断された場合
		return skillExec.currentPhase == Character::State::Action::CharacterSkillExecutionPhase::Completed
			|| skillExec.currentPhase == Character::State::Action::CharacterSkillExecutionPhase::Interrupted;
	default:
		return false;
	}
}
