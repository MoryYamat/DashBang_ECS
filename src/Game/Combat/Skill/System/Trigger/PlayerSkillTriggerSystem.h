// íœ—\’èF”p~DIntentResolver‚É‚æ‚éTrigger”»’è‚Ì•¡G‰»‚É”º‚¤Ó–±‚Ì”p~

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Game/Input/InputActionComponent.h"

#include "Game/Combat/Skill/System/Trigger/SkillInputMap.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::Trigger
{
	class PlayerSkillTriggerSystem
	{
	public:
		static void TriggerPlayerSkillsFromIntent(eNsECS::EntityMgr& ecs);
		
		// íœ—\’è:ŒÃ‚¢ƒo[ƒWƒ‡ƒ“
		// static void TriggerSkillsFromInput(eNsECS::EntityMgr& ecs, SkillInputMap& inputMap);

		// static void Update(ECS& ecs, SkillInputMap& SkillInputMap);
	};
}

