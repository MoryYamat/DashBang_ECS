//
#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Character/State/Rule/StateTransitionDatabase.hpp"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::State
{
	void InitCharacterStateTransitionDatabase(eNsECS::EntityMgr& ecs);

}