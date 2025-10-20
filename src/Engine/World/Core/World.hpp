#pragma once

#include "Engine/World/Core/Entity.hpp"

#include "Engine/World/Traits/ComponentTraits.hpp"

#include <cstdint>
#include <typeindex>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>
#include <cassert>
namespace Engine::World::Core
{
	class World
	{
	private:
		// 内部状態　(AoS)
		uint32_t nextId = 1;
		std::unordered_set<uint32_t> mAlive;
		std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::shared_ptr<void>>> mComponentPools;
		// std::unordered_map<uint32_t, uint32_t> mGenerations;

		std::unordered_map<std::type_index, std::shared_ptr<void>> mResources;

	public:
		World() = default;

		Entity Create();
		void Destroy(Entity e);
		bool IsAlive(Entity e) const;

		// component
		template<typename T, typename... Args>
		T& AddComponent(Entity e, Args&&... args)
		{
			assert(IsAlive(e));
			auto& bucket = mComponentPools[typeid(T)];

			auto it = bucket.find(e.id);
			assert(it == bucket.end() && "AddCompoent<T>: component already exists");

			auto sp = std::make_shared<T>(std::forward<Args>(args)...);
			auto& ref = *sp;
			bucket.emplace(e.id, sp);

			// Traits hook 
			// Traits::ComponentTraits<T>::OnAdd(ref);

			return *sp;
		}

		template<typename T>
		T* TryGet(Entity e)
		{
			auto it = mComponentPools.find(typeid(T));
			if (it == mComponentPools.end()) return nullptr;
			auto jt = it->second.find(e.id);
			if (jt == it->second.end()) return nullptr;
			return static_cast<T*>(jt->second.get());
		}

		template<typename T>
		const T* TryGet(Entity e) const
		{
			auto it = mComponentPools.find(typeid(T));
			if (it == mComponentPools.end()) return nullptr;
			auto jt = it->second.find(e.id);
			if (jt == it->second.end()) return nullptr;
			return static_cast<T*>(jt->second.get());
		}

		template<typename T>
		void RemoveComponent(Entity e)
		{
			auto it = mComponentPools.find(typeid(T));
			if (it == mComponentPools.end()) return;
			auto& bucket = it->second;
			if (auto jt = bucket.find(e.id); jt != bucket.end() && jt->second)
			{
				auto& ref = *static_cast<T*>(jt->second.get());

				//Traits hook
				// Traits::ComponentTraits<T>::OnRemove(ref);

				bucket.erase(jt);
			}

			if (bucket.empty()) mComponentPools.erase(it);
			// it->second.erase(e.id);
			// if (it->second.empty()) mComponentPools.erase(it);
		}

		//
		template<typename T>
		bool HasComponent(Entity e) const
		{
			auto it = mComponentPools.find(typeid(T));
			if (it == mComponentPools.end()) return false;
			return it->second.find(e.id) != it->second.end();
		}

		template<typename T>
		T& Get(Entity e)
		{
			auto* p = TryGet<T>(e);
			assert(p && "Get<T>: component missing");
			return *p;
		}

		// 生存Entityの列挙フック 
		template<typename Fn>
		void ForEachAlive(Fn&& fn) const
		{
			for (auto id : mAlive)
				fn(Entity{ id });
		}

		// ====================================================
		// Resouces
		template<typename T, typename... Args>
		T& CreateResource(Args&&... args)
		{
			auto& slot = mResources[typeid(T)];
			if (!slot) slot = std::make_shared<T>(std::forward<Args>(args)...);
			return *static_cast<T*>(slot.get());
		}

		template<typename T>
		T* TryGetResource()
		{
			auto it = mResources.find(typeid(T));
			return (it == mResources.end()) ? nullptr : static_cast<T*>(it->second.get());
		}

		template<typename T>
		const T* TryGetResource() const 
		{
			auto it = mResources.find(typeid(T));
			return (it == mResources.end()) ? nullptr : static_cast<const T*>(it->second.get());
		}

		template<typename T>
		bool HasResource() const
		{
			return mResources.count(typeid(T)) != 0;
		}

		template<typename T>
		T& GetResouce()
		{
			auto* p = TryGetResource<T>();
			assert(p && "GetResource<T>: missing");
			return *p;
		}

		template<typename T>
		void RemoveResource()
		{
			mResources.erase(typeid(T));
		}
	};
}

//World /
// Core /
//	Entity.h
//	World.h
//	World.cpp
//	WorldFwd.h
// Query /
//	View.h
//	Filter.h
// Traits /
//	ComponentTraits.h
// Resource /
//	Resources.h
