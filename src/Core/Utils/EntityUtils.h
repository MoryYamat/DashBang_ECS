// Functions that provide auxiliary functions for ECS
// ECS‚Ì•â•‹@”\‚ğ’ñ‹Ÿ‚·‚éŠÖ”ŒQ
#pragma once

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include <cstdint>
#include <string>

namespace EntityUtils
{
	uint32_t getEntityByName(ECS& ecs, const std::string& name);

}