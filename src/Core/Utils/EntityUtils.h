// Functions that provide auxiliary functions for ECS
// ECS�̕⏕�@�\��񋟂���֐��Q
#pragma once

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include <cstdint>
#include <string>

namespace EntityUtils
{
	uint32_t getEntityByName(ECS& ecs, const std::string& name);

}