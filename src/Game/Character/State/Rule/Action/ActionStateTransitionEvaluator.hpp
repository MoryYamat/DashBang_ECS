#pragma once


#include "Game/Character/State/Component/ActionStateComponent.hpp"
#include "Game/Character/State/Rule/Action/ActionStateTransitionRule.hpp"

#include "Game/Combat/Skill/Intent/Component/SkillIntentComponent.h"

#include <vector>

#include "Common/GameNamespaceDecl.h"

namespace Game::Character::State::Action
{
	bool AreConditionsMet
	(
		const gNsCharaActionState::CharacterActionStateComponent& action,
		const gNsSkillIntent::SkillIntentComponent& intent,
		float deltaTime,
		const std::vector<gNsCharaActionState::TransitionCondition>& conditions
	);

	bool EvaluateCondition
	(
		const gNsCharaActionState::CharacterActionStateComponent& action,
		const gNsSkillIntent::SkillIntentComponent& intent,
		float deltaTime,
		const gNsCharaActionState::TransitionCondition& condition
	);
}