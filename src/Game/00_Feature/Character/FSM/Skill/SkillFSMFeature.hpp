#pragma once
#include "Engine/ECS/EntityManager.h"
#include "Common/EngineNamespaceDecl.h"


namespace Game::Feature::Skill::FSM
{
	class SkillFSMFeature
	{
	public:
		static void UpdateSkillFSMSystem(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}