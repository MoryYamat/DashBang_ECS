#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Feature::Character::FSM
{
	class CCFSMFeature
	{
	public:
		static void InitCCFSMDefinitionDatabase(Engine::ECS::EntityMgr& ecs);

		static void UpdateCCFSMSystem(Engine::ECS::EntityMgr& ecs);

		static void UpdateCCFSMResolverSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);




		static void InitCCFSMDefinitionDatabase(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateCCFSMSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateCCFSMResolverSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}