#include "HitEventFeature.hpp"

#include "Game/Combat/HitEvent/Database/InitHitEventDatabase.hpp"

#include "Game/Combat/HitEvent/System/HitEventManager.hpp"

namespace Game::Feature::Combat
{
	using namespace Engine::ECS;

	using namespace Game::Combat;

	// Init
	void HitEventFeature::InitializeHitEventDatabase(EntityMgr& ecs)
	{
		Game::Combat::HitEvent::Init::InitHitEventDatabase(ecs);
	}

	void HitEventFeature::UpdateHitEventManager(EntityMgr& ecs)
	{
		HitEvent::System::HitEventManager(ecs);
	}
}