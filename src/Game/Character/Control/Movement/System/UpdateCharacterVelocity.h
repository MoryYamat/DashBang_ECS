//

#pragma once 

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::Movement
{
	void UpdateCharacterVelocity(eNsECS::EntityMgr& ecs);
}