#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::Combat
{
	
	class HitEventFeature
	{
	public:
		static void InitializeHitEventDatabase(Engine::ECS::EntityMgr& ecs);

		static void UpdateHitEventManager(Engine::ECS::EntityMgr& ecs);
	};
}