#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::System
{

	class WorldClockFeature
	{
	public:
		static void Init(Engine::ECS::EntityMgr& ecs);
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}