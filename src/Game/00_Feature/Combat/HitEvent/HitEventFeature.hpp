#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::Combat
{
	using namespace Engine::ECS;

	class HitEventFeature
	{
	public:
		static void InitializeHitEventDatabase(EntityMgr& ecs);
	};
}