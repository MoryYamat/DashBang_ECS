#pragma once 

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::State::Movement
{
	void InitCharaMovementStateTransitionDatabase(eNsECS::EntityMgr& ecs);
}