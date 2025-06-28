#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::ECS::System
{
	void UpdateLifetimeSystem(eNsECS::EntityMgr& ecs, float deltaTime);
}