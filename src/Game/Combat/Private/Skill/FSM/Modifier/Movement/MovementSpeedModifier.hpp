#pragma once

#include "Engine/ECS/EntityManager.h"



namespace Game::Combat::Skill::FSM::Modifier::Movement
{
	float CalcMovementSpeedMultiplierFromSkillFSM
	(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity entity
	);
}