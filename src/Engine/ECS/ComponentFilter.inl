#pragma once

#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <type_traits>
#include <vector>
#include <memory>

#include <cstddef>
#include <iostream>

#include "Common/EngineNamespaceDecl.h"

namespace Engine::ECS::Detail
{


	// AND: ‚·‚×‚Ä‚ÌŒ^‚ğ‚Â‚©
	template<typename... MustComponents>
	bool hasAllComponents(const std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::shared_ptr<void>>>& pools,
		uint32_t entityID)
	{
		//bool allFound = true;

		//(void)std::initializer_list<int>
		//{
		//	(allFound = allFound && [&]
		//		{
		//			std::type_index type = std::type_index(typeid(MustComponents));
		//			auto it = pools.find(type);
		//			return it != pools.end() && it->second.find(entityID) != it->second.end();
		//		}(), 0)...
		//};

		//return allFound;

		//return (... && [&] {
		//	auto poolIt = pools.find(std::type_index(typeid(MustComponents)));
		//	return poolIt != pools.end() && poolIt->second.find(entityID) != poolIt->second.end();
		//	}());


		return (...&& (// ’Z—•]‰¿‚É‚æ‚Á‚ÄŒø—¦‰»
			pools.count(std::type_index(typeid(MustComponents))) &&// ’Z—•]‰¿‚É‚æ‚Á‚Ä—áŠO‚É‚Í“ü‚ç‚È‚¢
			pools.at(std::type_index(typeid(MustComponents))).count(entityID)
			));
	}

	// OR: ‚¢‚¸‚ê‚©‚ÌŒ^‚ğ‚Â‚©
	template<typename... AnyComponents>
	bool hasAnyComponents(const std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::shared_ptr<void>>>& pools,
		uint32_t entityID)
	{
		//bool anyFound = false;

		//// •›ì—p‚ğ—˜—p
		//(void)std::initializer_list<int>{// intŒ^ƒŠƒXƒg
		//	(anyFound = anyFound || [&] // •›ì—p‚ÅŒvZŒ‹‰Ê‚ğ“¾‚éFanyFound || lambda() 
		//		{
		//			std::type_index type = std::type_index(typeid(AnyComponents));
		//			auto it = pools.find(type);
		//			return it != pools.end() && it->second.find(entityID) != it->second.end();
		//		}(), 0)...// ƒJƒ“ƒ}‰‰Zq (anyFound || lambda (), 0) => list—v‘f‚ğ‚·‚×‚Ä (int)0‚É‚µ‚Ä‚Â‚Ô‚·(’†g‚Í•s—v) void‚ÅÌ‚Ä‚é
		//};

		//return anyFound;

		//return (... || [&] {
		//	[pools, entityID]() {
		//		std::type_index type = std::type_index(typeid(AnyComponents));
		//		auto it = pools.find(type);
		//		return it != pools.end() && it->second.find(entityID) != it->second.end();
		//		}());

		return (... || (// ’Z—•]‰¿‚É‚æ‚Á‚ÄŒø—¦‰»
			pools.count(std::type_index(typeid(AnyComponents))) &&// ’Z—•]‰¿‚É‚æ‚Á‚Ä—áŠO‚É‚Í“ü‚ç‚È‚¢
			pools.at(std::type_index(typeid(AnyComponents))).count(entityID)
			));
	}
}