#pragma once


#include "Game/Character/State/Component/Action/ActionStateComponent.hpp"
#include "Game/Character/State/Rule/Action/ActionStateTransitionRule.hpp"

#include "Game/Character/State/Component/Action/Skill/CharacterSkillExecutionStateComponent.hpp"

#include "Game/Character/Control/Skill/Intent/SkillIntentComponent.hpp"

#include <vector>

#include "Common/GameNamespaceDecl.h"

namespace Game::Character::State::Action
{
	bool AreConditionsMet
	(
		const gNsCharaActionState::CharacterActionStateComponent& action,
		const gNsCharaActionState::CharacterSkillExecutionStateComponent& skillExec,
		float deltaTime,
		const std::vector<gNsCharaActionState::TransitionCondition>& conditions
	);

	bool EvaluateCondition
	(
		const gNsCharaActionState::CharacterActionStateComponent& action,
		const gNsCharaActionState::CharacterSkillExecutionStateComponent& skillExec,
		float deltaTime,
		const gNsCharaActionState::TransitionCondition& condition
	);


	// à»â∫îpé~ÅFçÌèúó\íË=> skillEcectionStateComponentÇ…äÓÇ√Ç≠èåèÇ…ïœçXÇµÇΩÇΩÇﬂ
	//bool AreConditionsMet
	//(
	//	const gNsCharaActionState::CharacterActionStateComponent& action,
	//	const gNsCharacterControlSkill::SkillIntentComponent& intent,
	//	float deltaTime,
	//	const std::vector<gNsCharaActionState::TransitionCondition>& conditions
	//);

	//bool EvaluateCondition
	//(
	//	const gNsCharaActionState::CharacterActionStateComponent& action,
	//	const gNsCharacterControlSkill::SkillIntentComponent& intent,
	//	float deltaTime,
	//	const gNsCharaActionState::TransitionCondition& condition
	//);
}