#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Character/FSM/Movement/StateModel/MovementFSMLeaseComponent.hpp"
#include "Game/Character/FSM/Movement/StateModel/MovementStateComponent.hpp"
#include "Game/Character/FSM/Movement/StateModel/MovementFSMInterferenceRequestComponent.hpp"

namespace Game::Character::FSM::Movement::Interference
{

	class MovementFSMInterferenceResolver
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs);

	private:
		static const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest* computeHighestPriorityRequest(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e,
			const Game::Character::FSM::Movement::StateModel::MovementFSMInterferenceRequestComponent& requestComp,
			Game::Character::FSM::Movement::MovementStateComponent& state,
			Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent& lease
			);

		// Interference受理 1フレームのみ
		static void acceptInterference(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e,
			const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest& req,
			Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent& lease,
			float clock
		);

		// 干渉動作の期限をコントロール
		static void updateInterference(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e,
			Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent& lease,
			float clock,
			float dt
		);
	};

}