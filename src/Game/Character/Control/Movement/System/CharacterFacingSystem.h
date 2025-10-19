//

#pragma once 

#include "Engine/ECS/EntityManager.h"

#include "Engine/InputManager/RawInputState.h"
#include "Engine/Graphics/Renderer/RenderContext.h"

namespace Game::Character::Movement
{
	// PlayerCharacterのfront向きを更新する
	void UpdatePlayerFacing(Engine::ECS::EntityMgr& ecs);

	// void UpdateAIFacing(ECS& ecs);
}