#pragma once

#include "Engine/ECS/EntityManager.h"



namespace Game::Character::State::Life
{
	class LifeStateSystem
	{
	public:
		static void UpdateStates(Engine::ECS::EntityMgr& ecs);
	};
}