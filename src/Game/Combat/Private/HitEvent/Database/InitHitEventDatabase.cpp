#include "InitHitEventDatabase.hpp"

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Private/HitEvent/Data/HitEvent.hpp"

#include "Game/Combat/Private/HitEvent/Database/HitEventDatabase.hpp"

#include <iostream>

namespace Game::Combat::HitEvent::Init
{
	using namespace Game::Combat::HitEvent::Database;

	void InitHitEventDatabase(Engine::ECS::EntityMgr& ecs)
	{
		auto& hitDb = ecs.createResource<HitEventDatabase>();

		std::cout << "[Init_HitEvent_DB]: The HitEventDatabase initialization completed successfully.\n";
	}
}