#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"


#include "Engine/ECS/EntityManager.h"

namespace Game::Input::Setting
{
	void InitSkillInputMap(Engine::ECS::EntityMgr& ecs);

	void InitSkillInputMap(Engine::WorldSystem::Core::WorldCtx& ctx);
}