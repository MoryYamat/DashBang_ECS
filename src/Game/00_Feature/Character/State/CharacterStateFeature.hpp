//
#pragma once

#include "Engine/ECS/EntityManager.h"


#include "Common/EngineNamespaceDecl.h"

namespace Game::Feature::Character
{
	class StateFeature
	{
	public:
		static void InitTransitionDatabase(eNsECS::EntityMgr& ecs);

		static void UpdateCharacterState(eNsECS::EntityMgr& ecs, float deltaTime);

		static void UPdateCharacterSkillExecutionState(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}