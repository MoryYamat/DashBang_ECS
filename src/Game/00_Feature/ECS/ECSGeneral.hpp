#pragma once


#include "Engine/ECS/EntityManager.h"




namespace Game::Feature::ECS
{
	class GameGeneralSytem
	{
	public:
		static void UpdateLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};

	// その他、必要なヘルパー関数やユーティリティ関数をここに追加
} // namespace Game::00_Feature::ECS