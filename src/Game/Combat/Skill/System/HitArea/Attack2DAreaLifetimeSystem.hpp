#pragma once

#include "Common/EngineNamespaceDecl.h"

#include "Engine/ECS/EntityManager.h"

namespace Game::Combat::Skill::System
{
	void UpdateAttack2DAreaLifetimeSystem(eNsECS::EntityMgr& ecs, float deltaTime);
}