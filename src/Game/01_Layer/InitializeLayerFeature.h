#pragma once

#include "Common/EngineNamespaceDecl.h"

#include "Engine/ECS/EntityManager.h"


namespace Game::Layer
{
	// 初期化フェーズ
	class InitializeLayerFeature
	{
	public:
		// 初期化順に注意


		// 遅らせ初期化
		static void DelayedInitialzation(eNsECS::EntityMgr& ecs);
	};
}