//

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Game/Input/InputActionComponent.h"

#include "Game/Combat/Skill/Trigger/SkillInputMap.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::Trigger
{
	class PlayerSkillTriggerSystem
	{
	public:
		static void TriggerPlayerSkillsFromIntent(eNsECS::EntityMgr& ecs);
		static void TriggerSkillsFromInput(eNsECS::EntityMgr& ecs, SkillInputMap& inputMap);

		// static void Update(ECS& ecs, SkillInputMap& SkillInputMap);
	};
}

