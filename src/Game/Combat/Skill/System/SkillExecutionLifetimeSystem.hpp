#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Combat::Skill::System
{
	void UpdateSkillExecutionLifetimeSystem(eNsECS::EntityMgr& ecs);
}