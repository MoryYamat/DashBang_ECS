#pragma once

// #include "Engine/ECS/EntityManager.h"

namespace Engine::ECS { class EntityMgr; }

namespace Game::Combat::HitEvent::Init
{

	void InitHitEventDatabase(Engine::ECS::EntityMgr& ecs);
}