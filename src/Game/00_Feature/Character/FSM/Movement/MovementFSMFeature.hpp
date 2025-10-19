#pragma once

#include "Engine/ECS/EntityManager.h"


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
	};
}