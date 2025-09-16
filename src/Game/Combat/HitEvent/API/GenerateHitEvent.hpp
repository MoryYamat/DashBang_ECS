#pragma once

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"


#include "Game/Combat/HitEvent/Data/HitEvent.hpp"
#include "Game/Combat/HitEvent/Database/HitEventDatabase.hpp"

#include "Common/EngineNamespaceDecl.h"

#include <cstdint>

namespace Game::Combat::HitEvent::API
{
	inline void GenerateHitEvent(eNsECS::EntityMgr& ecs,
		const eNsECS::Entity skill,
		const eNsECS::Entity target,
		const uint32_t skillId,
		const float now,
		Game::Combat::HitEvent::Database::HitEventDatabase& db
	)
	{
		Game::Combat::HitEvent::Data::HitEvent ev{};
		ev.skill = skill;
		ev.target = target;
		ev.skillId = skillId;
		ev.SpawnTime = now;

		db.push(std::move(ev));
	}
}