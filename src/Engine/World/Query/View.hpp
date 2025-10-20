#pragma once

#include "Engine/World/Core/WorldFacade.hpp"
#include <vector>


namespace Engine::World::Query
{
	// {e | P(e) = true}
	// 述語 pred(w, e) が true の Entity を返す
	template<typename Pred>
	std::vector<Core::Entity> ViewWhere(const Core::WorldRead& rw, Pred pred)
	{
		std::vector<Core::Entity> out;
		rw.ForEachAlive([&](Core::Entity e)
			{
				if (pred(rw, e)) out.push_back(e);
			});

		return out;
	}


	//template<typename... Cs>
	//std::vector<Core::Entity> ViewAll(const Core::WorldRead& rw)
	//{
	//	std::vector<Core::Entity> out;
	//	rw.ForEachAlive([&](Core::Entity e)
	//		{
	//			if ((rw.TryGet<Cs>(e) != nullptr && ...)) out.push_back(e);
	//		});

	//	return out;
	//}
}