// Input -> Intent for Skill input
#pragma once

#include "Common/EngineNamespaceDecl.h"

#include "Engine/ECS/EntityManager.h"

namespace Game::Input::Intent
{
	struct SkillIntentMappingSystem
	{
		static void UpdatePlayerSkillIntent(eNsECS::EntityMgr& ecs);
	};
}