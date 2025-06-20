#pragma once 

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::State::Life
{
	void InitLifeStateTransitionDatabase(eNsECS::EntityMgr& ecs);
}