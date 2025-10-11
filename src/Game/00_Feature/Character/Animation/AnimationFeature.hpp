#pragma once

#include "Engine/ECS/EntityManager.h"


namespace Game::Feature::Character::Animation
{
	class CharacterAnimationFeature
	{
	public:
		// locomotion animation
		static void InitProfileDatabase(Engine::ECS::EntityMgr& ecs);

		static void UpdateBuildAnimationQuery(Engine::ECS::EntityMgr& ecs);

		static void UpdateMovementAnimationResolver(Engine::ECS::EntityMgr& ecs);

		static void UpdateAnimationArbiterSystem(Engine::ECS::EntityMgr& ecs);

		static void UpdateApplyFinalAnimationDecision(Engine::ECS::EntityMgr& ecs);

		// skill animation
		static void InitSkillProfileDatabase(Engine::ECS::EntityMgr& ecs);
		static void UpdateSkillAnimationResolver(Engine::ECS::EntityMgr& ecs);
		static void UpdateBuildSkillAnimQuerySystem(Engine::ECS::EntityMgr& ecs);
	};
}