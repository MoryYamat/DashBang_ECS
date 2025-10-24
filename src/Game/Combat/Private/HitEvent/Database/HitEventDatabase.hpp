#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Private/HitEvent/Data/HitEvent.hpp"

#include <vector>

#include <algorithm> // remove_if

namespace Game::Combat::HitEvent::Database
{

	struct HitEventDatabase
	{

		
		std::vector<Data::HitEvent> items;
		Data::EventID nextId = 1;

		Data::EventID allocateId() { return nextId++; }
		// 自動採番
		void push(Data::HitEvent e)
		{ 
			if (e.eventId == 0) e.eventId = allocateId();
			items.push_back(std::move(e));
		}

		template<class Pred>
		void erase_if(Pred p)
		{
			items.erase(std::remove_if(items.begin(), items.end(), p), items.end());
		}

		void findEntityID(Engine::ECS::Entity a)
		{
			// if(items.)

		}
	};

	// read only
	[[nodiscard]]inline const HitEventDatabase& hitEventDatabase(const Engine::ECS::EntityMgr& ecs) noexcept
	{
			// ecs側でリソース存在確認(assert)
			return ecs.getResource<HitEventDatabase>();
	}

	// writable
	[[nodiscard]]inline HitEventDatabase& hitEventDatabase(Engine::ECS::EntityMgr& ecs) noexcept
	{
		return ecs.getResource<HitEventDatabase>();
	}

}