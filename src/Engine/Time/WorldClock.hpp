#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Engine::Time
{
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
		static void InitWorldClock(Engine::ECS::EntityMgr& ecs);
		static void TickWorldClock(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};

	// read only
	[[nodiscard]] inline const WorldClockData& worldClock(const Engine::ECS::EntityMgr& ecs) noexcept
	{
		return ecs.getResource<WorldClockData>();
	}

	// writable
	[[nodiscard]] inline WorldClockData& worldClock(Engine::ECS::EntityMgr& ecs) noexcept
	{
		return ecs.getResource<WorldClockData>();
	}
}