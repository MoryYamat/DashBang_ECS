#pragma once


#include "Engine/ECS/EntityManager.h"

namespace Game::Character::State::Action
{
	class ActionStateSystem
	{
	public:
		static void UpdateStates(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}