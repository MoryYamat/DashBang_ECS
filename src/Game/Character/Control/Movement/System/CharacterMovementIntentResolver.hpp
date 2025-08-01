#pragma once

#include "Engine/ECS/EntityManager.h"	
#include "Common/EngineNamespaceDecl.h"


namespace Game::Character::Control::Movement
{
	void UpdateMovementIntentResolverSystem(eNsECS::EntityMgr& ecs);
}