#pragma once

#include "Engine/ECS/EntityManager.h"



namespace Game::Feature::Character
{
	// Character Skill Feature
	class CharacterSkillFeature
	{
	public:
		static void UpdateCharacterSkillExecution(Engine::ECS::EntityMgr& ecs, float deltaTime);
		
		static void UpdateCharacterSkillIntentResolver(Engine::ECS::EntityMgr& ecs);
	};
} // namespace Game::Character::Control::Skill