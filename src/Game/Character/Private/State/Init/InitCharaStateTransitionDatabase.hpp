#pragma once 

#include "Engine/ECS/EntityManager.h"



namespace Game::Character::State::Life
{
	void InitLifeStateTransitionDatabase(Engine::ECS::EntityMgr& ecs);
}