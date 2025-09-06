#pragma once

#include "Engine/ECS/EntityManager.h"


namespace Game::Combat::HitEvent::Init
{
	using namespace Engine::ECS;

	void InitHitEventDatabase(EntityMgr& ecs);
}