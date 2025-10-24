// 

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Private/Skill/MasterData/SkillDatabase.h"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"



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


		//
		//
		static void InitializeSkillDatabase(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpateSkillIntent(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void TriggerSkillsFromIntent(Engine::WorldSystem::Core::WorldCtx& ctx);

		// 廃止
		// static void SkillGenerateSystem(Engine::ECS::EntityMgr& ecs);

		static void UpdateSkillTrajectorySystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateSkillPhaseSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateSkillExecutionLifetimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		// static void UpdateSkillAttackLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);


		static void UpdateHitoboxLifetimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}