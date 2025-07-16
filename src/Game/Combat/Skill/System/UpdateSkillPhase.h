// íœ—\’èFFSMƒVƒXƒeƒ€À‘•Œã”p~


#pragma once 

#include "Engine/ECS/EntityManager.h"
#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::System
{
	void UpdateSkillPhase(eNsECS::EntityMgr& ecs, float deltaTime);

	void UpdateSkillPhase(eNsECS::EntityMgr& ecs, float deltaTime, gNsSkillData::SkillDatabase& skillDB);

	bool AllAttacksDestroyed(eNsECS::EntityMgr& ecs, const gNsSkillComp::SkillInstanceComponent& instance);

	bool IsSkillInterrupted(const gNsSkillComp::SkillInstanceComponent& instance, eNsECS::EntityMgr& ecs);
}