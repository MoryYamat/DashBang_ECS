#pragma once

#include "Game/Character/State/Component/LifeStateComponent.hpp"

#include "Game/Character/State/Rule/Life/LifeStateTransitionRule.hpp"

#include "Game/Character/Stats/Component/CharacterStatsComponent.h"

#include "Common/GameNamespaceDecl.h"

namespace Game::Character::State::Life
{
	bool AreConditionsMet
	(
		const gNsCharaLifeState::CharacterLifeStateComponent& lifeComp,
		const gNsCharacterStats::CharacterStatsComponent& statsComp,
		const std::vector<gNsCharaLifeState::TransitionCondition>& conditions
	);

	bool EvaluateCondition
	(
		const gNsCharaLifeState::CharacterLifeStateComponent& lifeComp,
		const gNsCharacterStats::CharacterStatsComponent& statsComp,
		const gNsCharaLifeState::TransitionCondition& conditions
	);

}