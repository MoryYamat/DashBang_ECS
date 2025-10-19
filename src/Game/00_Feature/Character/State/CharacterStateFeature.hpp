//
#pragma once

#include "Engine/ECS/EntityManager.h"




namespace Game::Feature::Character
{
	class StateFeature
	{
	public:
		static void InitTransitionDatabase(Engine::ECS::EntityMgr& ecs);

		static void UpdateCharacterState(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void UPdateCharacterSkillExecutionState(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}