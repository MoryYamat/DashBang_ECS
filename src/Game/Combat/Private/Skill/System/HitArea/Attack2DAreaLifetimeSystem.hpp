#pragma once



#include "Engine/ECS/EntityManager.h"

namespace Game::Combat::Skill::System
{
	void UpdateAttack2DAreaLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);
}