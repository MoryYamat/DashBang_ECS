#pragma once

#include "Engine/ECS/EntityManager.h"

namespace Game::Character::Animation::Resolve::Skill
{
	struct SkillAnimResolverSystem
	{
		static void Update(Engine::ECS::EntityMgr& ecs);
	};
}