//
#include "UpdateSkillLifetimes.h"

#include "Game/SkillSystem/Component/Attack2DAreaComponent.h"
#include "Game/SkillSystem/Component/SkillInstanceComponent.h"

#include <iostream>

#include <vector>

void SkillSystem::Lifetime::CleanUpCompletedSkills(ECS& ecs)
{
	std::vector<Entity> toDestroyInstances;
	std::vector<Entity> toDestroyHitAreas;

	// パフォーマンスに関する懸念 (再び全コンポーネントを探索している)-> 解決策1. CleanUpCompletedSkill(Entity e)として再探索を避ける
	// パフォーマンスに関する懸念 (再び全コンポーネントを探索している)-> 解決策2. ecs.addComponent(e, PendingDestroyComponent{});として DestroySystemに統合する
	// パフォーマンスに関する懸念 (再び全コンポーネントを探索している)
	for (Entity e : ecs.view<SkillInstanceComponent>())
	{
		auto& skillInstance = ecs.get<SkillInstanceComponent>(e);

		if (skillInstance.phase == SkillPhase::Completed)
		{
			toDestroyInstances.push_back(e);

			for (Entity hitArea : skillInstance.spawnedHitAreas)
			{
				toDestroyHitAreas.push_back(hitArea);
			}
		}

	}

	for (Entity e : toDestroyHitAreas)
	{
		if (ecs.isAlive(e)) {
			ecs.destroyEntity(e);
			std::cout << "[SkillCleanup] Destroyed hit area: " << e.id << std::endl;
		}
	}

	for (Entity e : toDestroyInstances)
	{
		if (ecs.isAlive(e)) {
			ecs.destroyEntity(e);
			std::cout << "[SkillCleanup] Destroyed skill instance: " << e.id << std::endl;
		}
	}
}

void SkillSystem::Lifetime::UpdateSkillLifetimes(ECS& ecs, float deltaTime, SkillDatabase& skillDB)
{
	SkillSystem::Lifetime::updateAttack2DAreaCompLifetimes(ecs, deltaTime);
	SkillSystem::Lifetime::updateSkillInstanceCompLifetimes(ecs, deltaTime, skillDB);
}

void SkillSystem::Lifetime::updateAttack2DAreaCompLifetimes(ECS& ecs, float deltaTime)
{
	std::vector<Entity> toDestroy;

	for (Entity e : ecs.view<Attack2DAreaComponent>())
	{
		auto& area = ecs.get<Attack2DAreaComponent>(e);
		// lifetime を更新
		area.lifetime -= deltaTime;

		// std::cout << "[UpdateSkillLifetimes.cpp(update)] skill lifetme: " << area.lifetime << std::endl;

		// lifetimeが切れたEntityを収集
		if (area.lifetime <= 0.0f)
		{
			toDestroy.push_back(e);
		}
	}

	// 該当Entity をすべて削除
	for (Entity e : toDestroy)
	{
		std::cout << "[SkillLifetime] SkillInstanceComponent expired and removed: " << e.id << std::endl;
		ecs.destroyEntity(e);
	}
}

void SkillSystem::Lifetime::updateSkillInstanceCompLifetimes(ECS& ecs, float deltaTime, SkillDatabase& skillDB)
{
	std::vector<Entity> toDestroy;

	for (Entity e : ecs.view<SkillInstanceComponent>())
	{
		auto& skillInstance = ecs.get<SkillInstanceComponent>(e);
		skillInstance.timeSinceCast += deltaTime;

		const SkillDefinition& def = skillDB.Get(skillInstance.skillId);

		if (skillInstance.timeSinceCast >= def.duration)
		{
			toDestroy.push_back(e);
		}
	}

	for (Entity e: toDestroy)
	{
		{
			std::cout << "[SkillLifetime] SkillInstanceComponent expired and removed: " << e.id << std::endl;
			ecs.destroyEntity(e);
		} 
	}
}