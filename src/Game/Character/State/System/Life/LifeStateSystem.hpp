#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::State::Life
{
	class LifeStateSystem
	{
	public:
		static void UpdateStates(eNsECS::EntityMgr& ecs);
	};
}