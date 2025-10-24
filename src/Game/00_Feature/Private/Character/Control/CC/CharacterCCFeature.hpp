#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Feature::Character
{
	class CharacterCCFeature
	{

	public:
		static void InitCCAntiChainPolicy(Engine::ECS::EntityMgr& ecs);

		static void UpdateCCAntiChainSystem(Engine::ECS::EntityMgr& ecs);



		static void InitCCAntiChainPolicy(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateCCAntiChainSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}