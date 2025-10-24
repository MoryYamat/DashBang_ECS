#include "InitCharaStateTransitionDatabase.hpp"

#include "Game/Character/Private/State/Rule/Life/LifeStateTransitionDatabase.hpp"



#include <iostream>

void Game::Character::State::Life::InitLifeStateTransitionDatabase(Engine::ECS::EntityMgr& ecs)
{
	auto& db = ecs.createResource<Game::Character::State::Life::StateTransitionDatabase>();

	using State = Game::Character::State::Life::LifeState;
	using ConditionType = Game::Character::State::Life::TransitionConditionType;

	db.rules =
	{
		// Alive -> Dead: HPが0以下
		{
			.from = State::Alive,
			.to = State::Dead,
			.conditions =
			{
				{ ConditionType::HPIsZeroOrLess }
			}
		},

		// Dead -> Alive: 蘇生フラグ
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