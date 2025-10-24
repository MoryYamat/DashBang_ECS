#pragma once

#include "Engine/ECS/Entity.h"


#include <cstdint>

namespace Game::Combat::Skill::Component
{
	struct SkillOwnerComponent
	{
		Engine::ECS::Entity caster{};
		uint32_t skillId{ 0 };

	};
}