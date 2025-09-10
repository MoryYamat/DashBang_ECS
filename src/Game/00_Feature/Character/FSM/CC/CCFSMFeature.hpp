#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::Character::FSM
{
	class CCFSMFeature
	{
	public:
		static void InitCCFSMDefinitionDatabase(Engine::ECS::EntityMgr& ecs);

		static void UpdateCCFSMSystem(Engine::ECS::EntityMgr& ecs);

		static void UpdateCCFSMResolverSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}