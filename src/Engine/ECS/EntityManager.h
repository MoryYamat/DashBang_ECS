#pragma once

#include "Entity.h"

#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <type_traits>
#include <vector>
#include <memory>

#include <cstddef>
#include <iostream>

#include "Common/EngineNamespaceDecl.h"

namespace Engine::ECS
{
	// Entity Manager
	class EntityMgr
	{
	private:

		uint32_t nextEntityID = 1;

		// ※※Efficiency issues※※	
		// ※※Efficiency issues※※
		// ※※Efficiency issues※※
		// コンポーネントごとにエンティティを紐づける
		std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::shared_ptr<void>>> mComponentPools;
		// {type_index(Component), unordered_map{e.id, Components.data}}

	/// <summary>
	/// mComponentPools = {
	/// 	{ typeid(TransformComponent), { 1: data1, 3: data3 } },
	/// 	{ typeid(LogicComponent) , { 1: data1 } },
	/// 	{ typeid(AIComponent) , { 2: data2, 5: data5 } }
	/// </summary>

		// 
		std::unordered_set<uint32_t> mLivingEntities;

		// グローバルに存在する参照すべき共有情報
		std::unordered_map<std::type_index, std::shared_ptr<void>> mResources;

	public:
		// create
		Entity createEntity()
		{
			Entity e{ nextEntityID++ };
			mLivingEntities.insert(e.id);
			return e;
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

		// search and safety valve
		// 検索と安全弁
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

		// すべてのComponentに付随する情報の削除
		void Clear();

		// 任意のEntityに付随するComponentの削除(Componentを探索し，該当Entityがあれば削除する)
		void destroyEntity(Entity e)
		{
			for (auto& [type, entityMap] : mComponentPools)
			{
				//std::size_t erased = entityMap.erase(e.id);
				//if (erased > 0)
				//{
				//	std::cout << "[EntityManager(destroy)] Deletion of Component """ << type.name() << " for " << e.id << " completed successfully\n";
				//}
				entityMap.erase(e.id);
			}
			mLivingEntities.erase(e.id);
		}

		bool isAlive(Entity e) const
		{
			return mLivingEntities.count(e.id) > 0;
		}

		// Resource 登録・取得API

		// Resourceを登録
		//template<typename T>
		//void addResource(const T& resource)
		//{
		//	mResources[std::type_index(typeid(T))] = std::make_shared<T>(resource);
		//}

		// Resourceを登録
		template<typename T, typename... Args>
		T& createResource(Args&&... args)
		{
			// インスタンス生成
			auto res = std::make_shared<T>(std::forward<Args>(args)...);
			// 新規作成
			mResources[std::type_index(typeid(T))] = res;
			// shared_ptr<T>を返す
			return *res;
		}

		// mResourceを取得
		template<typename T>
		T& getResource()
		{
			return *std::static_pointer_cast<T>(mResources[std::type_index(typeid(T))]);
		}

		// Resouceが存在するか確認
		template<typename T>
		bool hasResource() const
		{
			return mResources.count(std::type_index(typeid(T))) > 0;
		}
	};
}

