#pragma once

#include "Engine/ECS/Entity.h"

#include "Common/EngineNamespaceDecl.h"
#include <cstdint>

namespace Game::Combat::Skill::Component
{
	struct SkillOwnerComponent
	{
		eNsECS::Entity caster;
		uint32_t skillId;
	};
}