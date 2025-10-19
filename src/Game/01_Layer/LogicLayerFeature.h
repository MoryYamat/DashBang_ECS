#pragma once

#include "Engine/ECS/EntityManager.h"





namespace Game::Layer
{
	class LogicLayerFeature
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}