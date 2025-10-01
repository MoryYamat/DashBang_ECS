#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Character/FSM/Movement/StateModel/MovementFSMInterferenceRequestComponent.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

namespace Game::Character::FSM::Movement::API::External
{
	inline Game::Character::FSM::Movement::StateModel::MovementFSMInterferenceRequestComponent*
		tryGetMovementInterferenceRequestComponent(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity e
	)
	{
		if (!ecs.hasComponent<Game::Character::FSM::Movement::StateModel::MovementFSMInterferenceRequestComponent>(e))
		{
			Engine::ECS::Ops::Add< Game::Character::FSM::Movement::StateModel::MovementFSMInterferenceRequestComponent>
				(ecs, e, Game::Character::FSM::Movement::StateModel::MovementFSMInterferenceRequestComponent{});
			//ecs.addComponent(e, Game::Character::FSM::Movement::StateModel::MovementFSMInterferenceRequestComponent{});
		}

		return &ecs.get< Game::Character::FSM::Movement::StateModel::MovementFSMInterferenceRequestComponent>(e);
		// return nullptr;
	};
}