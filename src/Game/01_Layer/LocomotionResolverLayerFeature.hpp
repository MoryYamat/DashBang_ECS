#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Layer
{
	class LocomotionResolverLayerFeature
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs);
	};
}