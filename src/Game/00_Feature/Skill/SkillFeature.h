// 

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Feature
{
	class SkillFeature
	{
	public:
		static void InitializeSkillDatabase(eNsECS::EntityMgr& ecs);

		static void UpateSkillIntent(eNsECS::EntityMgr& ecs);

		static void SkillGenerateSystem(eNsECS::EntityMgr& ecs, gNsSkillData::SkillDatabase& skillDB);

		static void UpdateSkillTrajectorySystem(eNsECS::EntityMgr& ecs, float deltaTime);

		static void UpdateSkillPhaseSystem(eNsECS::EntityMgr& ecs, float deltaTime, gNsSkillData::SkillDatabase& skillDB);

	};
}