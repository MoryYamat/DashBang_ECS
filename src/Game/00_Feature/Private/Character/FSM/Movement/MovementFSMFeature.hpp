#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Feature::Character::FSM
{
	class MovementFSMFeature
	{
	public:
		static void InitializeMovementFSMDefinition(Engine::ECS::EntityMgr& ecs);

		static void UpdateMovementIntentResolver(Engine::ECS::EntityMgr& ecs);

		static void UpdateMovementFSMSystem(Engine::ECS::EntityMgr& ecs);

		static void UpdateMovementFSMResolverSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void UpdateMovementFSMScopedSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void UpdateMovementFSMInterferenceResolver(Engine::ECS::EntityMgr& ecs);




		static void InitializeMovementFSMDefinition(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateMovementIntentResolver(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateMovementFSMSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateMovementFSMResolverSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateMovementFSMScopedSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateMovementFSMInterferenceResolver(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}