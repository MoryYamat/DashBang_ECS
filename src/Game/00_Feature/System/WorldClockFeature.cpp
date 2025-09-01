#include "WorldClockFeature.hpp"


#include "Engine/Time/WorldClock.hpp"

namespace Game::Feature::System
{
	using WorldClock = Engine::Time::WorldClockSystem;

	void WorldClockFeature::Init(EntityMgr& ecs)
	{
		WorldClock::InitWorldClock(ecs);
	}

	void WorldClockFeature::Update(EntityMgr& ecs, float deltaTime)
	{
		WorldClock::TickWorldClock(ecs, deltaTime);
	}
}