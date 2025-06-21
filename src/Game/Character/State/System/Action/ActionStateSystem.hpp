#pragma once

#include "Common/EngineNamespaceDecl.h"
#include "Engine/ECS/EntityManager.h"

namespace Game::Character::State::Action
{
	class ActionStateSystem
	{
	public:
		static void UpdateStates(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}