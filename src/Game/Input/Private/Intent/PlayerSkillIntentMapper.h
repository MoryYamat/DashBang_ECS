// Input -> Intent for Skill input
#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"


#include "Engine/ECS/EntityManager.h"

namespace Game::Input::Intent
{
	struct SkillIntentMappingSystem
	{
		static void UpdatePlayerSkillIntent(Engine::ECS::EntityMgr& ecs);

		static void UpdatePlayerSkillIntent(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}