#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"


namespace Game::Character::FSM::CC::Reset
{

	struct IResetHandler
	{

		virtual ~IResetHandler() = default;

		virtual void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e
		) const = 0;
	};

}