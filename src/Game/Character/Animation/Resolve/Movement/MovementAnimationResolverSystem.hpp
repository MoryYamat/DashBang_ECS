#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Character::Animation::Resolve::Movement
{
	// 再生したいアニメーションの候補を決める
	struct MovementAnimationResolverSystem
	{
		static void Update(Engine::ECS::EntityMgr& ecs);
	};
}