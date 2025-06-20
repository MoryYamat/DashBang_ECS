#include "InitCharaMoveStateTransitionDatabase.hpp"

#include "Game/Character/State/Rule/Movement/MovementStateTransitionDatabase.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Character::State::Movement::InitCharaMovementStateTransitionDatabase(eNsECS::EntityMgr& ecs)
{
	auto& db = ecs.createResource<gNsCharaMoveState::StateTransitionDatabase>();

	using State = gNsCharaMoveState::MovementState;
	using ConditionType = gNsCharaMoveState::TransitionConditionType;

	db.rules =
	{
		// Alive -> Dead: HPÇ™0à»â∫
		{
			.from = State::Idle,
			.to = State::Moving,
			.conditions =
			{
				{ ConditionType::IsActualMoving}
			}
		},

		// Dead -> Alive: ëhê∂ÉtÉâÉO
		{
			.from = State::Moving,
			.to = State::Idle,
			.conditions =
			{
				{ ConditionType::IsStationaly }
			}
		}
	};

	std::cout << "[InitLifeStateTransitionDatabase.cpp]: Initialization of the Life state transition condition database completed successfully.\n";

}