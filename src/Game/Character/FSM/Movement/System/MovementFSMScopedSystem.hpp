#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::FSM::Movement::System
{
	struct MovementFSMScopedEffectSystem
	{
		static void Update(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}