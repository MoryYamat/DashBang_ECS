#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::Character
{
	class CharacterCCFeature
	{

	public:
		static void InitCCAntiChainPolicy(Engine::ECS::EntityMgr& ecs);

		static void UpdateCCAntiChainSystem(Engine::ECS::EntityMgr& ecs);
	};
}