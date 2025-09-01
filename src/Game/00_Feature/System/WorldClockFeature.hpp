#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::System
{
	using namespace Engine::ECS;

	class WorldClockFeature
	{
	public:
		static void Init(EntityMgr& ecs);
		static void Update(EntityMgr& ecs, float deltaTime);
	};
}