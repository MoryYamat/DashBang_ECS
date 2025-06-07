// Intent -> Velocity -> Position

#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::Character
{
	class MovementFeature
	{
	public:
		// intent
		static void UpdateMovementIntent(eNsECS::EntityMgr& ecs);

		// expression
		static void UpdateVelocity(eNsECS::EntityMgr& ecs);

		// logic
		static void UpdateLogicFacing(eNsECS::EntityMgr& ecs);
		static void UpdateLogicPosition(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}
