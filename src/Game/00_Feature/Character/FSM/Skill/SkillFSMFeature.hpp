#pragma once
#include "Engine/ECS/EntityManager.h"
#include "Common/EngineNamespaceDecl.h"


namespace Game::Feature::Character::Skill::FSM
{
	class SkillFSMFeature
	{
	public:
		static void UpdateSkillFSMSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void UpdateSkillFSMResolverSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void UpdateSkillInterferenceResolver(Engine::ECS::EntityMgr& ecs);
	};
}