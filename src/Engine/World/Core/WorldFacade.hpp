#pragma once

#include "Engine/World/Core/World.hpp"

#include <utility>

namespace Engine::World::Core
{
	struct WorldRead
	{
		const World& w;

		template<typename T>
		const T* TryGet(Entity e) const { return w.TryGet<T>(e); }

		template<typename T>
		bool Has(Entity e) const { return w.HasComponent<T>(e); }

		template<typename Fn>
		void ForEachAlive(Fn&& fn) const { w.ForEachAlive(std::forward<Fn>(fn)); }


		// resource
		template<typename T>
		T* TryGetResource() const { return w.TryGetResource<T>(); }

		template<typename T>
		bool HasResource() const { return w.HasResource<T>(); }
	};

	struct WorldWrite
	{
		World& w;

		template<typename T, typename... Args>
		T& Add(Entity e, Args&&... args) { return w.AddComponent<T>(e, std::forward<Args>(args)...); }

		template<typename T>
		void Remove(Entity e) { w.RemoveComponent<T>(e); }

		// 読み取り
		template<typename T> bool Has(Entity e) { return w.HasComponent<T>(e); }
		template<typename T> T* TryGet(Entity e) { return w.TryGet<T>(e); }
		template<typename Fn> void ForEachAlive(Fn&& fn) { w.ForEachAlive(std::forward<Fn>(fn)); }


		// resource
		template<typename T, typename... Args>
		T& CreateResource(Args&&... args) { return w.CreateResource<T>(std::forward<Args>(args)...); }

		template<typename T>
		T* TryGetResource() { return w.TryGetResource<T>(); }

		template<typename T>
		const T* TryGetResource() const { return w.TryGetResource<T>(); }

		template<typename T>
		T& GetResource() { return w.GetResouce<T>(); }

		template<typename T>
		const T& GetResource() const { return w.GetResouce<T>(); }

		template<typename T>
		void RemoveResource() { w.RemoveResource<T>(); }
	};
}