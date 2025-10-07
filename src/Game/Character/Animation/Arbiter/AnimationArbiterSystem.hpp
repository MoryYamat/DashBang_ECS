#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Character::Animation::Arbiter
{
	struct AnimationArbiterSystem
	{
		static void Update(Engine::ECS::EntityMgr& ecs);
	};
}