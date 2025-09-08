#include "WorldClock.hpp"

namespace Engine::Time
{
	using namespace Engine::ECS;

	void WorldClockSystem::InitWorldClock(EntityMgr& ecs)
	{
		auto& clock = ecs.createResource<WorldClockData>();
	}

	void WorldClockSystem::TickWorldClock(EntityMgr& ecs, float deltaTime)
	{
		auto& clock = ecs.getOrCreateResource<WorldClockData>();
		clock.dt = clock.paused ? 0.0f : deltaTime * clock.scale;
		clock.now += clock.dt;
	}
}