#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Character::Animation::Apply
{
	struct ApplyFinalAnimationDecisionSystem
	{
		static void Update(Engine::ECS::EntityMgr& ecs);
	};
}
