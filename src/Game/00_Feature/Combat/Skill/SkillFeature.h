// 

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Feature::Combat
{
	class SkillFeature
	{
	public:
		static void InitializeSkillDatabase(eNsECS::EntityMgr& ecs);

		static void UpateSkillIntent(eNsECS::EntityMgr& ecs);

		static void TriggerSkillsFromIntent(eNsECS::EntityMgr& ecs);

		// îpé~
		// static void SkillGenerateSystem(eNsECS::EntityMgr& ecs);

		static void UpdateSkillTrajectorySystem(eNsECS::EntityMgr& ecs, float deltaTime);

		static void UpdateSkillPhaseSystem(eNsECS::EntityMgr& ecs, float deltaTime);

		static void UpdateSkillAttackLifetimeSystem(eNsECS::EntityMgr& ecs, float deltaTime);

	};
}