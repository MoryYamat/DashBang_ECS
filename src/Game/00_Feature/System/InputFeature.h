//

#pragma once



#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::Setting
{
	class InputFeature
	{
	public:
		static void InitInputMapping(Engine::ECS::EntityMgr& ecs);

		static void InitSkillInputMapping(Engine::ECS::EntityMgr& ecs);

	};
}