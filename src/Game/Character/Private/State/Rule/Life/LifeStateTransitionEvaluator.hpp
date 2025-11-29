#pragma once

#include "Game/Character/Private/State/Component/LifeStateComponent.hpp"

#include "Game/Character/Private/State/Rule/Life/LifeStateTransitionRule.hpp"

#include "Game/Character/Stats/Public/StatsComponent.hpp"



namespace Game::Character::State::Life
{
	bool AreConditionsMet
	(
		const Game::Character::State::Life::CharacterLifeStateComponent& lifeComp,
		const Game::Character::Stats::CharacterStatsComponent& statsComp,
		const std::vector<Game::Character::State::Life::TransitionCondition>& conditions
	);

	bool EvaluateCondition
	(
		const Game::Character::State::Life::CharacterLifeStateComponent& lifeComp,
		const Game::Character::Stats::CharacterStatsComponent& statsComp,
		const Game::Character::State::Life::TransitionCondition& conditions
	);

}