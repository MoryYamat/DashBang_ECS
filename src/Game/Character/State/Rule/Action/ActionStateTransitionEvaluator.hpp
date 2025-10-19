#pragma once


#include "Game/Character/State/Component/Action/ActionStateComponent.hpp"
#include "Game/Character/State/Rule/Action/ActionStateTransitionRule.hpp"

#include "Game/Character/State/Component/Action/Skill/CharacterSkillExecutionStateComponent.hpp"

#include "Game/Character/Control/Skill/Intent/SkillIntentComponent.hpp"

#include <vector>



namespace Game::Character::State::Action
{
	bool AreConditionsMet
	(
		const Game::Character::State::Action::CharacterActionStateComponent& action,
		const Game::Character::State::Action::CharacterSkillExecutionStateComponent& skillExec,
		float deltaTime,
		const std::vector<Game::Character::State::Action::TransitionCondition>& conditions
	);

	bool EvaluateCondition
	(
		const Game::Character::State::Action::CharacterActionStateComponent& action,
		const Game::Character::State::Action::CharacterSkillExecutionStateComponent& skillExec,
		float deltaTime,
		const Game::Character::State::Action::TransitionCondition& condition
	);


	// 以下廃止：削除予定=> skillEcectionStateComponentに基づく条件に変更したため
	//bool AreConditionsMet
	//(
	//	const Game::Character::State::Action::CharacterActionStateComponent& action,
	//	const Game::Character::Control::Skill::SkillIntentComponent& intent,
	//	float deltaTime,
	//	const std::vector<Game::Character::State::Action::TransitionCondition>& conditions
	//);

	//bool EvaluateCondition
	//(
	//	const Game::Character::State::Action::CharacterActionStateComponent& action,
	//	const Game::Character::Control::Skill::SkillIntentComponent& intent,
	//	float deltaTime,
	//	const Game::Character::State::Action::TransitionCondition& condition
	//);
}