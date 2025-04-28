// Utils for Debugging 

#pragma once

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include <iostream>


namespace DebugUtils
{

	// A template that returns the name of the Component with Entity.id.
	// Entity.id �����R���|�[�l���g�̖��O��Ԃ��e���v���[�g
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
	// ����̃R���|�[�l���g�̏W�������G���e�B�e�B����������e���v���[�g
	template<typename... Components>
	void printEntityComponents(ECS& ecs, Entity entity)
	{
		(printEntityComponentIfExist<Components>(ecs, entity), ...);
	}
}