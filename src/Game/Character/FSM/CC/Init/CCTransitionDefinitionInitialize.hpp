#pragma once
#include "Engine/ECS/EntityManager.h"
#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::FSM::CC
{
	void InitCCTransitionDefinitionDatabase(eNsECS::EntityMgr& ecs);
}