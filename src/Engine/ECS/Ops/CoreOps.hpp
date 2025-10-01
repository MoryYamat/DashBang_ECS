#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Engine::ECS::Ops
{
	// add 
	template<typename T, typename... Args>
	inline T& Add(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity e, Args&&... args)
	{
		return ecs.addComponent<T>(e, std::forward<Args>(args)...);
	}

	template<typename T>
	inline void Remove(Engine::ECS::EntityMgr& ecs, Entity e)
	{
		ecs.removeComponent<T>(e);
	}


}