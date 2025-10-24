#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Character::Animation::Arbiter
{
	// move / skill / cc どのアニメーションを優先するかを解決する
	struct AnimationArbiterSystem
	{
		static void Update(Engine::ECS::EntityMgr& ecs);

	};
}