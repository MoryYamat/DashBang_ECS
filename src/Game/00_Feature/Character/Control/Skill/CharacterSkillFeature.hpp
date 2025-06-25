#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Feature::Character
{
	// Character Skill Feature
	class CharacterSkillFeature
	{
	public:
		static void UpdateCharacterSkillExecution(eNsECS::EntityMgr& ecs, float deltaTime);
		
		static void UpdateCharacterSkillIntentResolver(eNsECS::EntityMgr& ecs);
	};
} // namespace Game::Character::Control::Skill