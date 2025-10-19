// 削除予定：廃止．IntentResolverによるTrigger判定の複雑化に伴う責務の廃止

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Game/Input/InputActionComponent.h"

#include "Game/Combat/Skill/System/Trigger/SkillInputMap.h"




namespace Game::Combat::Skill::Trigger
{
	class PlayerSkillTriggerSystem
	{
	public:
		static void TriggerPlayerSkillsFromIntent(Engine::ECS::EntityMgr& ecs);
		
		// 削除予定:古いバージョン
		// static void TriggerSkillsFromInput(Engine::ECS::EntityMgr& ecs, SkillInputMap& inputMap);

		// static void Update(ECS& ecs, SkillInputMap& SkillInputMap);
	};
}

