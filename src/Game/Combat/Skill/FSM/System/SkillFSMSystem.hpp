#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Common/EngineNamespaceDecl.h"


namespace Game::Combat::Skill::FSM
{
	void UpdateSkillFSMSystem(eNsECS::EntityMgr& ecs, float deltaTime);
}