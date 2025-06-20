#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::State::Movement
{
	class MovementStateSystem
	{
	public:
		static void UpdateStates(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}