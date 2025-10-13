#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Layer::Debug
{
	class DebugLayerFeature
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs);
	};
}