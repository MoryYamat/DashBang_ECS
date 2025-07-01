//
#include "UpdateSkillLifetimes.h"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"
#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"

#include <iostream>

#include <vector>

void Game::Combat::Skill::System::CleanUpCompletedSkills(eNsECS::EntityMgr& ecs)
{
	std::vector<eNsECS::Entity> toDestroyInstances;
	std::vector<eNsECS::Entity> toDestroyHitAreas;

	// パフォーマンスに関する懸念 (再び全コンポーネントを探索している)-> 解決策1. CleanUpCompletedSkill(Entity e)として再探索を避ける
	// パフォーマンスに関する懸念 (再び全コンポーネントを探索している)-> 解決策2. ecs.addComponent(e, PendingDestroyComponent{});として DestroySystemに統合する
	// パフォーマンスに関する懸念 (再び全コンポーネントを探索している)
	for (eNsECS::Entity e : ecs.view<gNsSkillComp::SkillInstanceComponent>())
	{
		auto& skillInstance = ecs.get<gNsSkillComp::SkillInstanceComponent>(e);

		if (skillInstance.phase == gNsSkillComp::SkillPhase::Completed)
		{
			toDestroyInstances.push_back(e);

			for (eNsECS::Entity hitArea : skillInstance.spawnedHitAreas)
			{
				toDestroyHitAreas.push_back(hitArea);
			}
		}

	}

	for (eNsECS::Entity e : toDestroyHitAreas)
	{
		if (ecs.isAlive(e)) {
			ecs.destroyEntity(e);
			std::cout << "[SkillCleanup] Destroyed hit area: " << e.id << std::endl;
		}
	}

	for (eNsECS::Entity e : toDestroyInstances)
	{
		if (ecs.isAlive(e)) {
			ecs.destroyEntity(e);
			std::cout << "[SkillCleanup] Destroyed skill instance: " << e.id << std::endl;
		}
	}
}

void Game::Combat::Skill::System::UpdateSkillLifetimes(eNsECS::EntityMgr& ecs, float deltaTime, gNsSkillData::SkillDatabase& skillDB)
{
	updateAttack2DAreaCompLifetimes(ecs, deltaTime);
	updateSkillInstanceCompLifetimes(ecs, deltaTime, skillDB);
}

void Game::Combat::Skill::System::updateAttack2DAreaCompLifetimes(eNsECS::EntityMgr& ecs, float deltaTime)
{
	std::vector<eNsECS::Entity> toDestroy;

	for (eNsECS::Entity e : ecs.view<gNsSkillComp::Attack2DAreaComponent>())
	{
		auto& area = ecs.get<gNsSkillComp::Attack2DAreaComponent>(e);
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
	for (eNsECS::Entity e : toDestroy)
	{
		std::cout << "[SkillLifetime] SkillInstanceComponent expired and removed: " << e.id << std::endl;
		ecs.destroyEntity(e);
	}
}

void Game::Combat::Skill::System::updateSkillInstanceCompLifetimes(eNsECS::EntityMgr& ecs, float deltaTime, gNsSkillData::SkillDatabase& skillDB)
{
	std::vector<eNsECS::Entity> toDestroy;

	for (eNsECS::Entity e : ecs.view<gNsSkillComp::SkillInstanceComponent>())
	{
		auto& skillInstance = ecs.get<gNsSkillComp::SkillInstanceComponent>(e);
		skillInstance.timeSinceCast += deltaTime;

		const gNsSkillData::SkillDefinition& def = skillDB.Get(skillInstance.skillId);

		if (skillInstance.timeSinceCast >= def.attackSpec.lifetime.duration)
		{
			toDestroy.push_back(e);
		}
	}

	for (eNsECS::Entity e: toDestroy)
	{
		{
			std::cout << "[SkillLifetime] SkillInstanceComponent expired and removed: " << e.id << std::endl;
			ecs.destroyEntity(e);
		} 
	}
}