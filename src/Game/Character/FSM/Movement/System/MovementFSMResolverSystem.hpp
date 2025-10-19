#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Character/FSM/Movement/StateModel/MovementStateComponent.hpp"

#include "Game/Character/FSM/Movement/StateModel/MovementFSMLeaseComponent.hpp"

#include "Game/Character/FSM/Movement/Database/MovementFSMDatabase.hpp"

#include <typeindex>


namespace Game::Character::FSM::Movement::System
{
	class MovementFSMResolverSystem
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);

	private:
		static bool tryApplyForcedTransition
		(
			Engine::ECS::EntityMgr& ecs,
			const Engine::ECS::Entity e,
			Game::Character::FSM::Movement::MovementStateComponent& state,
			const Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent& lease,
			const Game::Character::FSM::Movement::Database::MovementFSMDatabase& db
		);

		static bool applyStateUpdate(
			Game::Character::FSM::Movement::MovementStateComponent& state,
			std::type_index to
		);

		static void runMovementEffects
		(
			Engine::ECS::EntityMgr& ecs,
			const Engine::ECS::Entity e,
			Game::Character::FSM::Movement::MovementStateComponent& state,
			const Game::Character::FSM::Movement::Database::MovementFSMDatabase& db
		);
	};
}