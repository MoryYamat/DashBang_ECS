#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Combat::Skill::FSM::Modifier::Movement
{
	float CalcMovementSpeedMultiplierFromSkillFSM
	(
		eNsECS::EntityMgr& ecs,
		eNsECS::Entity entity
	);
}