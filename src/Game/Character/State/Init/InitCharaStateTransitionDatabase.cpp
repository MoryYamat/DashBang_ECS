#include "InitCharaStateTransitionDatabase.hpp"

#include "Game/Character/State/Rule/Life/LifeStateTransitionDatabase.hpp"

#include "Common/GameNamespaceDecl.h"

#include <iostream>

void Game::Character::State::Life::InitLifeStateTransitionDatabase(eNsECS::EntityMgr& ecs)
{
	auto& db = ecs.createResource<gNsCharaLifeState::StateTransitionDatabase>();

	using State = gNsCharaLifeState::LifeState;
	using ConditionType = gNsCharaLifeState::TransitionConditionType;

	db.rules =
	{
		// Alive -> Dead: HPÇ™0à»â∫
		{
			.from = State::Alive,
			.to = State::Dead,
			.conditions =
			{
				{ ConditionType::HPIsZeroOrLess }
			}
		},

		// Dead -> Alive: ëhê∂ÉtÉâÉO
		{
			.from = State::Dead,
			.to = State::Alive,
			.conditions =
			{
				{ ConditionType::ExplicitReviveFlag }
			}
		}
	};

	std::cout << "[InitLifeStateTransitionDatabase.cpp]: Initialization of the Life state transition condition database completed successfully.\n";
}