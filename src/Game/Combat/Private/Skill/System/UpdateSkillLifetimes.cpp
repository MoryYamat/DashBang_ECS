//
#include "UpdateSkillLifetimes.h"

#include "Game/Combat/Private/Skill/Component/Attack2DAreaComponent.h"
#include "Game/Combat/Private/Skill/Component/SkillInstanceComponent.h"

#include <iostream>

#include <vector>


// 削除予定：型ベースFSM導入後廃止予定
void Game::Combat::Skill::System::CleanUpCompletedSkills(Engine::ECS::EntityMgr& ecs)
{
	//std::vector<Engine::ECS::Entity> toDestroyInstances;
	//std::vector<Engine::ECS::Entity> toDestroyHitAreas;

	//// パフォーマンスに関する懸念 (再び全コンポーネントを探索している)-> 解決策1. CleanUpCompletedSkill(Entity e)として再探索を避ける
	//// パフォーマンスに関する懸念 (再び全コンポーネントを探索している)-> 解決策2. ecs.addComponent(e, PendingDestroyComponent{});として DestroySystemに統合する
	//// パフォーマンスに関する懸念 (再び全コンポーネントを探索している)
	//for (Engine::ECS::Entity e : ecs.view<Game::Combat::Skill::Component::SkillInstanceComponent>())
	//{
	//	auto& skillInstance = ecs.get<Game::Combat::Skill::Component::SkillInstanceComponent>(e);

	//	if (skillInstance.phase == Game::Combat::Skill::Component::SkillPhase::Completed)
	//	{
	//		toDestroyInstances.push_back(e);

	//		for (Engine::ECS::Entity hitArea : skillInstance.spawnedHitAreas)
	//		{
	//			toDestroyHitAreas.push_back(hitArea);
	//		}
	//	}

	//}

	//for (Engine::ECS::Entity e : toDestroyHitAreas)
	//{
	//	if (ecs.isAlive(e)) {
	//		ecs.destroyEntity(e);
	//		std::cout << "[SkillCleanup] Destroyed hit area: " << e.id << std::endl;
	//	}
	//}

	//for (Engine::ECS::Entity e : toDestroyInstances)
	//{
	//	if (ecs.isAlive(e)) {
	//		ecs.destroyEntity(e);
	//		std::cout << "[SkillCleanup] Destroyed skill instance: " << e.id << std::endl;
	//	}
	//}
}


// 削除予定：型ベースFSM導入後廃止予定
void Game::Combat::Skill::System::UpdateSkillLifetimes(Engine::ECS::EntityMgr& ecs, float deltaTime, Game::Combat::Skill::Data::SkillDatabase& skillDB)
{
	//updateAttack2DAreaCompLifetimes(ecs, deltaTime);
	//updateSkillInstanceCompLifetimes(ecs, deltaTime, skillDB);
}


// 削除予定：型ベースFSM導入後廃止予定
void Game::Combat::Skill::System::updateAttack2DAreaCompLifetimes(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	//std::vector<Engine::ECS::Entity> toDestroy;

	//for (Engine::ECS::Entity e : ecs.view<Game::Combat::Skill::Component::Attack2DAreaComponent>())
	//{
	//	auto& area = ecs.get<Game::Combat::Skill::Component::Attack2DAreaComponent>(e);
	//	// lifetime を更新
	//	area.lifetime -= deltaTime;

	//	// std::cout << "[UpdateSkillLifetimes.cpp(update)] skill lifetme: " << area.lifetime << std::endl;

	//	// lifetimeが切れたEntityを収集
	//	if (area.lifetime <= 0.0f)
	//	{
	//		toDestroy.push_back(e);
	//	}
	//}

	//// 該当Entity をすべて削除
	//for (Engine::ECS::Entity e : toDestroy)
	//{
	//	std::cout << "[SkillLifetime] SkillInstanceComponent expired and removed: " << e.id << std::endl;
	//	ecs.destroyEntity(e);
	//}
}


// 削除予定：型ベースFSM導入後廃止予定
void Game::Combat::Skill::System::updateSkillInstanceCompLifetimes(Engine::ECS::EntityMgr& ecs, float deltaTime, Game::Combat::Skill::Data::SkillDatabase& skillDB)
{
	//std::vector<Engine::ECS::Entity> toDestroy;

	//for (Engine::ECS::Entity e : ecs.view<Game::Combat::Skill::Component::SkillInstanceComponent>())
	//{
	//	auto& skillInstance = ecs.get<Game::Combat::Skill::Component::SkillInstanceComponent>(e);
	//	skillInstance.timeSinceCast += deltaTime;

	//	const Game::Combat::Skill::Data::SkillDefinition& def = skillDB.Get(skillInstance.skillId);

	//	if (skillInstance.timeSinceCast >= def.attackSpec.lifetime.duration)
	//	{
	//		toDestroy.push_back(e);
	//	}
	//}

	//for (Engine::ECS::Entity e: toDestroy)
	//{
	//	{
	//		std::cout << "[SkillLifetime] SkillInstanceComponent expired and removed: " << e.id << std::endl;
	//		ecs.destroyEntity(e);
	//	} 
	//}
}