#pragma once

#include "Entity.h"

#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <type_traits>
#include <vector>
#include <memory>


class ECS
{
private:

	uint32_t nextEntityID = 1;

	// ※※Efficiency issues※※	
	// ※※Efficiency issues※※
	// ※※Efficiency issues※※
	// Entityごとにコンポーネントを紐づける
	std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::shared_ptr<void>>> mComponentPools;
	// {type_index(Component), unordered_map{e.id, Components.data}}

public:
	// create
	Entity createEntity()
	{
		return Entity{ nextEntityID++ };
	}

	// add
	template<typename T>
	void addComponent(Entity e, T component)
	{
		const std::type_index type = std::type_index(typeid(T));

		if (mComponentPools.find(type) == mComponentPools.end())
		{
			//{ EntityID, ... }
			mComponentPools[type] = std::unordered_map<uint32_t, std::shared_ptr<void>>();
		}

		mComponentPools[type][e.id] = std::make_shared<T>(component);
	}

	// get
	template<typename T>
	T& get(Entity e)
	{
		std::type_index type = std::type_index(typeid(T));
		return *std::static_pointer_cast<T>(mComponentPools[type][e.id]);
	}

	// search
	template<typename... Components>
	std::vector<Entity> view()
	{
		std::vector<Entity> result;

		using FirstComponent = typename std::tuple_element<0, std::tuple<Components...>>::type;
		const std::type_index firstType = std::type_index(typeid(FirstComponent));

		if (mComponentPools.find(firstType) == mComponentPools.end())
		{
			return result;
		}

		//　事前探索
		const auto& base = mComponentPools[firstType];

		for (const auto& [entityID, _] : base)
		{
			bool hasAll = (... && (
				mComponentPools.count(std::type_index(typeid(Components))) &&
				mComponentPools[std::type_index(typeid(Components))].count(entityID)
				));

			if (hasAll)
			{
				result.push_back(Entity{ entityID });
			}
		}

		return result;

	}

	template<typename T>
	bool hasComponent(Entity e) const
	{
		// key
		std::type_index type = std::type_index(typeid(T));

		// キーで検索
		auto poolIt = mComponentPools.find(type);
		if (poolIt == mComponentPools.end())
		{
			return false;
		}

		// サブunordered_mapを取得
		const auto& entityMap = poolIt->second;
		// 検索
		return entityMap.find(e.id) != entityMap.end();
	}

	void Clear();
};