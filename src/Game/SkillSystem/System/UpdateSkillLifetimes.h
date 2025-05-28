// Attack2DAreaComponent（などスキルの効果として生成されたエンティティ） の lifetime を管理・終了させる処理

#pragma once

#include "Core/ECS/EntityManager.h"


#include "Game/SkillSystem/MasterData/SkillDatabase.h"

namespace SkillSystem::Lifetime
{
	void CleanUpCompletedSkills(ECS& ecs);

	void UpdateSkillLifetimes(ECS& ecs, float deltaTime, SkillDatabase& skillDB);

	// スキル状態導入前(最新)
	void updateAttack2DAreaCompLifetimes(ECS& ecs, float deltaTime);
	// 導入後
	// void updateAttack2DAreaCompLifetimes(ECS& ecs);

	// スキル状態導入前
	void updateSkillInstanceCompLifetimes(ECS& ecs, float deltaTime, SkillDatabase& skillDB);
	// スキル状態導入後(最新)
	// void updateSkillInstanceCompLifetimes(ECS& ecs);
}