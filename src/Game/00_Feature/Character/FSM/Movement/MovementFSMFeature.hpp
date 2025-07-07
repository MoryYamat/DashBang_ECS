#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Common/EngineNamespaceDecl.h"

namespace Game::Feature::Character::FSM
{
	class MovementFSMFeature
	{
	public:
		static void InitializeMovementFSMDefinition(eNsECS::EntityMgr& ecs);

		static void UpdateMovementFSMSystem(eNsECS::EntityMgr& ecs);
	};
}