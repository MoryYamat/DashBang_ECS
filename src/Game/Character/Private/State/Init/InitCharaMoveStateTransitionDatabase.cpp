#include "InitCharaMoveStateTransitionDatabase.hpp"

#include "Game/Character/Private/State/Rule/Movement/MovementStateTransitionDatabase.hpp"



void Game::Character::State::Movement::InitCharaMovementStateTransitionDatabase(Engine::ECS::EntityMgr& ecs)
{
	auto& db = ecs.createResource<Game::Character::State::Movement::StateTransitionDatabase>();

	using State = Game::Character::State::Movement::MovementState;
	using ConditionType = Game::Character::State::Movement::TransitionConditionType;

	db.rules =
	{
		// Alive -> Dead: HPが0以下
		{
			.from = State::Idle,
			.to = State::Moving,
			.conditions =
			{
				{ ConditionType::IsActualMoving}
			}
		},

		// Dead -> Alive: 蘇生フラグ
		{
			.from = State::Moving,
			.to = State::Idle,
			.conditions =
			{
				{ ConditionType::IsStationaly }
			}
		}
	};

	std::cout << "[InitCharaMovementStateTransitionDatabase.cpp]: Move state transition condition database initialization completed successfully.\n";

}