// Input -> Intent for Skill input
#pragma once



#include "Engine/ECS/EntityManager.h"

namespace Game::Input::Intent
{
	struct SkillIntentMappingSystem
	{
		static void UpdatePlayerSkillIntent(Engine::ECS::EntityMgr& ecs);
	};
}