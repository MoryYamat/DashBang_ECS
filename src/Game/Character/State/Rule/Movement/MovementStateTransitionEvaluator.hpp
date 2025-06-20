#pragma once

#include "Common/GameNamespaceDecl.h"
#include "Common/EngineNamespaceDecl.h"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/State/Component/MovementStateComponent.hpp"
#include "Game/Character/State/Rule/Movement/MovementStateTranstionRule.hpp"

#include <vector>

namespace Game::Character::State::Movement
{
	bool AreConditionsMet
	(
		const gNsCharaMoveState::CharacterMovementStateComponent& movement,
		const eNsLogic2DComp::Velocity2DComponent& vel,
		float deltaTime,
		const std::vector<gNsCharaMoveState::TransitionCondition>& conditions
	);

	bool EvaluateCondition
	(
		const eNsLogic2DComp::Velocity2DComponent& vel,
		float deltaTime,
		const gNsCharaMoveState::TransitionCondition& cond
	);
}