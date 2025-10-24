#pragma once

#include "Engine/ECS/EntityManager.h"


#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Layer
{
	class StateLayerFeature
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);

	};
}