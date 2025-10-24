// 

// Attack2DAreaComponent（などスキルの効果として生成されたエンティティ） の lifetime を管理・終了させる処理

#pragma once

#include "Engine/ECS/EntityManager.h"


#include "Game/Combat/Private/Skill/MasterData/SkillDatabase.h"





// (削除予定) Entityはフラグ管理と削除をGlobalSystemで行うため未使用 (現在未使用)
// (削除予定) Entityはフラグ管理と削除をGlobalSystemで行うため未使用 (現在未使用)
// (削除予定) Entityはフラグ管理と削除をGlobalSystemで行うため未使用 (現在未使用)
// (削除予定) Entityはフラグ管理と削除をGlobalSystemで行うため未使用 (現在未使用)
namespace Game::Combat::Skill::System
{
	void CleanUpCompletedSkills(Engine::ECS::EntityMgr& ecs);

	void UpdateSkillLifetimes(Engine::ECS::EntityMgr& ecs, float deltaTime, Game::Combat::Skill::Data::SkillDatabase& skillDB);

	// スキル状態導入前(最新)
	void updateAttack2DAreaCompLifetimes(Engine::ECS::EntityMgr& ecs, float deltaTime);
	// 導入後
	// void updateAttack2DAreaCompLifetimes(ECS& ecs);

	// スキル状態導入前
	void updateSkillInstanceCompLifetimes(Engine::ECS::EntityMgr& ecs, float deltaTime, Game::Combat::Skill::Data::SkillDatabase& skillDB);
	// スキル状態導入後(最新)
	// void updateSkillInstanceCompLifetimes(ECS& ecs);

}