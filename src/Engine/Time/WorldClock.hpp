#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Engine::Time
{
	using namespace Engine::ECS;

	struct WorldClockData
	{
		float now = 0.0f; // 単調増加の世界時刻(秒) max: 3.402823466 E + 38
		float dt = 0.0f; // 直近フレームのΔt()
		float scale = 1.0f; // スローや倍速用
		bool paused = false;// ポーズ用
	};

	class WorldClockSystem
	{
	public:
		static void InitWorldClock(EntityMgr& ecs);
		static void TickWorldClock(EntityMgr& ecs, float deltaTime);
	};

}