// Intent -> Velocity -> Position

#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature
{
	class MovementFeature
	{
	public:
		static void Update(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}
