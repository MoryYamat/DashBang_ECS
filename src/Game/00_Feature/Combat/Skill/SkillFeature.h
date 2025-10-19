// 

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"




namespace Game::Feature::Combat
{
	class SkillFeature
	{
	public:
		static void InitializeSkillDatabase(Engine::ECS::EntityMgr& ecs);

		static void UpateSkillIntent(Engine::ECS::EntityMgr& ecs);

		static void TriggerSkillsFromIntent(Engine::ECS::EntityMgr& ecs);

		// 廃止
		// static void SkillGenerateSystem(Engine::ECS::EntityMgr& ecs);

		static void UpdateSkillTrajectorySystem(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void UpdateSkillPhaseSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void UpdateSkillExecutionLifetimeSystem(Engine::ECS::EntityMgr& ecs);

		// static void UpdateSkillAttackLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);


		static void UpdateHitoboxLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);

	};
}