#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Entity.h"

namespace Game::Combat::HitEvent::System
{
	
	void HitEventManager(Engine::ECS::EntityMgr& ecs);

}