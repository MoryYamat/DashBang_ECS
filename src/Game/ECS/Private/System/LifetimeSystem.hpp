#pragma once

#include "Engine/ECS/EntityManager.h"



namespace Game::ECS::System
{
	void UpdateLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);
}