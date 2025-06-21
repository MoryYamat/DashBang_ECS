#pragma once 

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::State::Action
{
	void InitCharaActionStateTransitionDatabase(eNsECS::EntityMgr& ecs);
}