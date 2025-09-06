#include "HitEventFeature.hpp"

#include "Game/Combat/HitEvent/Database/InitHitEventDatabase.hpp"

namespace Game::Feature::Combat
{

	// Init
	void HitEventFeature::InitializeHitEventDatabase(EntityMgr& ecs)
	{
		Game::Combat::HitEvent::Init::InitHitEventDatabase(ecs);
	}
}