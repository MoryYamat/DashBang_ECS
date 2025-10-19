// Intent -> Velocity -> Position

#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::Character
{
	class MovementFeature
	{
	public:
		// intent
		static void UpdateMovementIntent(Engine::ECS::EntityMgr& ecs);

		// expression
		static void UpdateVelocity(Engine::ECS::EntityMgr& ecs);

		// logic
		static void UpdateLogicFacing(Engine::ECS::EntityMgr& ecs);
		static void UpdateLogicPosition(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}
