#pragma once

#include "Engine/ECS/EntityManager.h"



namespace Game::Character::FSM::Movement::System
{
	struct MovementFSMScopedEffectSystem
	{
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}