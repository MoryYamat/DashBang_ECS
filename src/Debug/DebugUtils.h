// Utils for Debugging 

#pragma once

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include <glm/glm.hpp>

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

	// 呼び出し、マクロ定義から、`DEBUG_LOG(msg)`
	// Call, from the macro definition, `DEBUG_LOG(msg)`
	inline void debugLog(const std::string& message, const char* functionName)
	{
		std::cout << "[Debug] [" << functionName << "]: " << message << std::endl;
	}


	// position log 2D
	inline void LogPosition(const std::string& tag, const glm::vec2& pos)
	{
		std::cout << "[" << tag << "] x: " << pos.x << " z: " << pos.y << "\n";
	}
	// position log 3d
	inline void LogPosition(const std::string& tag, const glm::vec3& pos)
	{
		std::cout << "[" << tag << "] x: " << pos.x << " y: " << pos.y << " z: " << pos.z << "\n";
	}

	// General Logs
	inline void GeneralLog(const std::string& tag, const std::string& contents)
	{
		std::cout << "[" << tag << "]: " << contents << "\n";
	}
}

#ifdef DEBUG
#define DEBUG_LOG(msg) DebugUtils::debugLog(msg,__FUNCTION__)
#else
#define DEBUG_LOG(msg)
#endif