#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Combat::Skill::Context
{
	class SkillExecutionContextSystem
	{
	public:
		void Update(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}