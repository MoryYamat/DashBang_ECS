//

#pragma once

#include "Core/ECS/EntityManager.h"
#include "Core/ECS/Component/InputActionComponent.h"

#include "Game/SkillSystem/Trigger/SkillInputMap.h"

namespace SkillSystem::Trigger
{
	class PlayerSkillTriggerSystem
	{
	public:
		static void TriggerSkillsFromInput(ECS& ecs, SkillInputMap& inputMap);

		// static void Update(ECS& ecs, SkillInputMap& SkillInputMap);
	};
}

