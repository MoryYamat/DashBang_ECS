// inside Logic Layer 


#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Collision::System
{
	void CollisionSyncSystem(eNsECS::EntityMgr& ecs);
}