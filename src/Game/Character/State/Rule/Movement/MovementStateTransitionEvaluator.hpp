#pragma once




#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/State/Component/MovementStateComponent.hpp"
#include "Game/Character/State/Rule/Movement/MovementStateTranstionRule.hpp"

#include <vector>

namespace Game::Character::State::Movement
{
	bool AreConditionsMet
	(
		const Game::Character::State::Movement::CharacterMovementStateComponent& movement,
		const Engine::ECS::Component::Logic2D::Velocity2DComponent& vel,
		float deltaTime,
		const std::vector<Game::Character::State::Movement::TransitionCondition>& conditions
	);

	bool EvaluateCondition
	(
		const Engine::ECS::Component::Logic2D::Velocity2DComponent& vel,
		float deltaTime,
		const Game::Character::State::Movement::TransitionCondition& cond
	);
}