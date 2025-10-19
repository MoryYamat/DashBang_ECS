#pragma once
#include "Engine/ECS/EntityManager.h"



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