// Utils for Debugging 

#pragma once

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include "Game/CollisionLogic/Data/CollisionResultStorage.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

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

	// log of vector
	template<typename T>
	inline void LogVector_string(const std::string& tag, const T& vec)
	{
		std::cout << "[" << tag << "] " << glm::to_string(vec) << "\n";
	}

	// position log 2D
	inline void LogVector(const std::string& tag, const glm::vec2& pos)
	{
		std::cout << "[" << tag << "] x: " << pos.x << " z: " << pos.y << "\n";
	}
	// position log 3d
	inline void LogVector(const std::string& tag, const glm::vec3& pos)
	{
		std::cout << "[" << tag << "] x: " << pos.x << " y: " << pos.y << " z: " << pos.z << "\n";
	}

	// General Logs
	inline void GeneralLog(const std::string& tag, const std::string& contents)
	{
		std::cout << "[" << tag << "]: " << contents << "\n";
	}
}

namespace DebugUtils::DebugDraw
{
	void DrawCross(const glm::vec3& pos, float size, const glm::vec3& color);

	void DrawCircle2D(const glm::vec2& centerXZ, float radius, const glm::vec3& color, int segments = 32);
	void DrawFilledCircle2D(const glm::vec2& centerXZ, float radius, const glm::vec4& color, int segments = 32);

	void DrawQuad(const glm::vec2& center, float size, const glm::vec3& color);

	void DrawFilledQuad(const glm::vec2& center, float size, glm::vec3& color);

	void DrawTileOutline(const glm::vec2& minXZ, const glm::vec2& maxXZ,const glm::vec3& color);

	void DrawLine2D(const glm::vec2& a, const glm::vec2 b, const glm::vec3& color);
	
	void DrawSector2D(const glm::vec2 centerXZ, const glm::vec2 direction, const float radius, const float angle, const glm::vec3& color);
	void DrawFilledSector2D(const glm::vec2 centerXZ, const glm::vec2 direction, const float radius, const float angle, const glm::vec4& color);
	void DrawFilledSector2D(const glm::vec2 centerXZ, const float rotation, const float radius, const float angle, const glm::vec4& color);

	void DrawFilledRect2D(const glm::vec2 centerXZ, const glm::vec2 direction,const float width, const float height, const glm::vec4& color);
	void DrawFilledRect2DOffeset(const glm::vec2 centerXZ, const glm::vec2 direction,const float width, const float height, const glm::vec4& color);
}

#ifdef DEBUG
#define DEBUG_LOG(msg) DebugUtils::debugLog(msg,__FUNCTION__)
#else
#define DEBUG_LOG(msg)
#endif