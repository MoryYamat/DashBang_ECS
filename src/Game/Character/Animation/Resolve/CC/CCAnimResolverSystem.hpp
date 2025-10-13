#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Character::Animation::Resolve::CC
{
	struct CCAnimResolverSystem
	{
		static void Update(Engine::ECS::EntityMgr& ecs);
	};
}