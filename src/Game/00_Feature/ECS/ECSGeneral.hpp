#pragma once


#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"


namespace Game::Feature::ECS
{
	class GameGeneralSytem
	{
	public:
		static void UpdateLifetimeSystem(eNsECS::EntityMgr& ecs, float deltaTime);
	};

	// その他、必要なヘルパー関数やユーティリティ関数をここに追加
} // namespace Game::00_Feature::ECS