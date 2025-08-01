//

#pragma once 

#include "Engine/ECS/EntityManager.h"

#include "Engine/InputManager/RawInputState.h"
#include "Engine/Graphics/Renderer/RenderContext.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Character::Movement
{
	// PlayerCharacter‚ÌfrontŒü‚«‚ğXV‚·‚é
	void UpdatePlayerFacing(eNsECS::EntityMgr& ecs);

	// void UpdateAIFacing(ECS& ecs);
}