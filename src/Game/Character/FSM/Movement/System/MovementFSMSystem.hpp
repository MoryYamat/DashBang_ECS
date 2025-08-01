#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::FSM::Movement
{
	void UpdateMovementFSMSystem(eNsECS::EntityMgr& ecs);
}