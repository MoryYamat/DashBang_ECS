//

#pragma once 

#include "Core/ECS/EntityManager.h"
#include "Game/SkillSystem/MasterData/SkillDatabase.h"

#include "Game/SkillSystem/Component/SkillInstanceComponent.h"

namespace SkillSystem::Phase
{
	void UpdateSkillPhase(ECS& ecs, float deltaTime, SkillDatabase& skillDB);

}