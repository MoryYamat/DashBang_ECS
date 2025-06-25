#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::Control::Skill
{
	void UpdateCharacterSkillExecution(eNsECS::EntityMgr& ecs, float deltaTime);
}