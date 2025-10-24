//

#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"


#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::Setting
{
	class InputFeature
	{
	public:
		static void InitInputMapping(Engine::ECS::EntityMgr& ecs);

		static void InitSkillInputMapping(Engine::ECS::EntityMgr& ecs);


		//
		//

		static void InitInputMapping(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void InitSkillInputMapping(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}