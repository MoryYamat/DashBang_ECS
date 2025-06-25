#pragma once


#include "Engine/ECS/EntityManager.h"	
#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::Control::Skill
{
	// キャラクターのスキルの意図を反映するかしないか判定するシステム
	void UpdateCharacterSkillIntentResovlver(eNsECS::EntityMgr& ecs);
}