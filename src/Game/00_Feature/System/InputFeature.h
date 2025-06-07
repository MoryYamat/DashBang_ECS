//

#pragma once

#include "Common/EngineNamespaceDecl.h"

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature::Setting
{
	class InputFeature
	{
	public:
		static void InitInputMapping(eNsECS::EntityMgr& ecs);

		static void InitSkillInputMapping(eNsECS::EntityMgr& ecs);

	};
}