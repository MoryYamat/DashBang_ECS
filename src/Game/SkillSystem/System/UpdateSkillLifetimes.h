// Attack2DAreaComponent（などスキルの効果として生成されたエンティティ） の lifetime を管理・終了させる処理

#pragma once

#include "Core/ECS/EntityManager.h"


#include "Game/SkillSystem/MasterData/SkillDatabase.h"

namespace SkillSystem::Lifetime
{
	void UpdateSkillLifetimes(ECS& ecs, float deltaTime, SkillDatabase& skillDB);

	void updateAttack2DAreaCompLifetimes(ECS& ecs, float deltaTime);

	void updateSkillInstanceCompLifetimes(ECS& ecs, float deltaTime, SkillDatabase& skillDB);
}