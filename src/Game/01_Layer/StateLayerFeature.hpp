#pragma once

#include "Engine/ECS/EntityManager.h"




namespace Game::Layer
{
	class StateLayerFeature
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}