// Functions that provide auxiliary functions for ECS
// ECSの補助機能を提供する関数群
#pragma once

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include <cstdint>
#include <string>

namespace EntityUtils
{
	//uint32_t getEntityByName(ECS& ecs, const std::string& name);

	// For name resolution
	Entity getEntityByName(ECS& ecs, const std::string& name);

}