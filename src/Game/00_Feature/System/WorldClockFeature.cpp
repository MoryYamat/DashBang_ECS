#include "WorldClockFeature.hpp"


#include "Engine/Time/Private/WorldClock.hpp"

namespace Game::Feature::System
{
	using WorldClock = Engine::Time::WorldClockSystem;

	void WorldClockFeature::Init(Engine::ECS::EntityMgr& ecs)
	{
		WorldClock::InitWorldClock(ecs);
	}

	void WorldClockFeature::Update(Engine::ECS::EntityMgr& ecs, float deltaTime)
	{
		WorldClock::TickWorldClock(ecs, deltaTime);
	}
}