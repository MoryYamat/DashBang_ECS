#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Character/FSM/Movement/StateModel/MovementFSMLeaseComponent.hpp"
#include "Game/Character/FSM/Movement/StateModel/MovementStateComponent.hpp"
#include "Game/Character/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"

namespace Game::Character::FSM::Movement::Interference
{
	using namespace Engine::ECS;
	using namespace Game::Character::FSM::Interference::Core::Data;
	using namespace Game::Character::FSM::Movement::StateModel;
	using namespace Game::Character::FSM::Movement;

	class MovementFSMInterferenceResolver
	{
	public:
		static void Update(EntityMgr& ecs, float deltaTime);

	private:
		static void resolveMovementFSMInterference(
			EntityMgr& ecs,
			Entity e, 
			FSMInterferenceRequestComponent requestComp,
			MovementStateComponent state,
			MovementFSMLeaseComponent lease
			);
	};

}