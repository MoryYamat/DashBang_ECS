// Utils for Debugging 

#pragma once

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include <iostream>


namespace DebugUtils
{

	// A template that returns the name of the Component with Entity.id.
	// Entity.id を持つコンポーネントの名前を返すテンプレート
	template<typename T>
	void printEntityComponentIfExist(ECS& ecs, Entity entity)
	{
		if (ecs.hasComponent<T>(entity))
		{
			std::cout << "[DebugUtils.h] Entity ID: " << entity.id << " have  "
				<< typeid(T).name() << std::endl;
		}
	}

	// A template to find entities with a particular set of components
	// 特定のコンポーネントの集合をもつエンティティを検索するテンプレート
	template<typename... Components>
	void printEntityComponents(ECS& ecs, Entity entity)
	{
		(printEntityComponentIfExist<Components>(ecs, entity), ...);
	}
}