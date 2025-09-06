#pragma once

#include "Game/Combat/HitEvent/Data/HitEvent.hpp"

#include <vector>

namespace Game::Combat::HitEvent::Database
{
	using namespace Game::Combat::HitEvent::Data;

	struct HitEventDatabase
	{
		std::vector<HitEvent> items;
		EventID nextId = 1;

		EventID allocateId() { return nextId++; }
		// 自動採番
		void push(HitEvent e) 
		{ 
			if (e.eventId == 0) e.eventId = allocateId();
			items.push_back(std::move(e));
		}

		template<class Pred>
		void erase_if(Pred p)
		{
			items.erase(std::remove_if(items.begin(), items.end(), p), items.end());
		}
	};
}