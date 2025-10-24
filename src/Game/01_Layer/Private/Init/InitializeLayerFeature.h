#pragma once



#include "Engine/ECS/EntityManager.h"

#include "Engine/Public/EngineFwd.hpp"

namespace Game::Layer
{
	// 初期化フェーズ
	class InitializeLayerFeature
	{
	public:
		// 初期化順に注意
		// 遅らせ初期化
		static void DelayedInitialzation(Engine::ECS::EntityMgr& ecs);

		static void DelayedInitialization(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}