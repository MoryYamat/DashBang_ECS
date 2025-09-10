#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Combat::HitEvent::System
{
	using namespace Engine::ECS;

	void HitEventManager(EntityMgr& ecs);
}