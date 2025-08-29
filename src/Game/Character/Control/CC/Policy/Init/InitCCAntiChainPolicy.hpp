#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Character::Control::CC::Policy
{
	using namespace Engine::ECS;

	void InitCCAntiChainPolicy(EntityMgr& ecs);
}