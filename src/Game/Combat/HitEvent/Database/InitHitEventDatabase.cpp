#include "InitHitEventDatabase.hpp"

#include "Game/Combat/HitEvent/Data/HitEvent.hpp"

#include "Game/Combat/HitEvent/Database/HitEventDatabase.hpp"


namespace Game::Combat::HitEvent::Init
{
	using namespace Game::Combat::HitEvent::Database;

	void InitHitEventDatabase(EntityMgr& ecs)
	{
		auto& hitDb = ecs.createResource<HitEventDatabase>();
	}
}