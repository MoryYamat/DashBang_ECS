#pragma once

#include "Engine/ECS/EntityManager.h"


namespace Game::Feature::Character::Animation
{
	class CharacterAnimationFeature
	{
	public:
		static void InitProfileDatabase(Engine::ECS::EntityMgr& ecs);

		static void UpdateBuildAnimationQuery(Engine::ECS::EntityMgr& ecs);

		static void UpdateMovementAnimationResolver(Engine::ECS::EntityMgr& ecs);

		static void UpdateAnimationArbiterSystem(Engine::ECS::EntityMgr& ecs);

		static void UpdateApplyFinalAnimationDecision(Engine::ECS::EntityMgr& ecs);
	};
}