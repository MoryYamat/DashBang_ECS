//

#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"


#include "Engine/ECS/EntityManager.h"

namespace Game::Input::Setting
{
	void InitInputMap(Engine::ECS::EntityMgr& ecs);

	void InitInputMap(Engine::WorldSystem::Core::WorldCtx& ctx);
}