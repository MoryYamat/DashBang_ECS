#pragma once


#include "Engine/ECS/EntityManager.h"	
#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::Control::Skill
{
	// 
	void UpdateSkillResolverSystem(eNsECS::EntityMgr& ecs);

	// キャラクターのスキルの意図を反映するかしないか判定するシステム
	void UpdateCharacterSkillIntentResovlver(eNsECS::EntityMgr& ecs);
}