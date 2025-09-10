#include "InitHitEventDatabase.hpp"

#include "Game/Combat/HitEvent/Data/HitEvent.hpp"

#include "Game/Combat/HitEvent/Database/HitEventDatabase.hpp"

#include <iostream>

namespace Game::Combat::HitEvent::Init
{
	using namespace Game::Combat::HitEvent::Database;

	void InitHitEventDatabase(EntityMgr& ecs)
	{
		auto& hitDb = ecs.createResource<HitEventDatabase>();

		std::cout << "[Init_HitEvent_DB]: The HitEventDatabase initialization completed successfully.\n";
	}
}