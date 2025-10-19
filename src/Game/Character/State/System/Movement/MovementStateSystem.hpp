#pragma once

#include "Engine/ECS/EntityManager.h"



namespace Game::Character::State::Movement
{
	class MovementStateSystem
	{
	public:
		static void UpdateStates(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}