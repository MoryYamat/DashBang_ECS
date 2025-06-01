// Attack2DAreaComponent（などスキルの効果として生成されたエンティティ） の lifetime を管理・終了させる処理

#pragma once

#include "Engine/ECS/EntityManager.h"


#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"


// (削除予定) Entityはフラグ管理と削除をGlobalSystemで行うため未使用 (現在未使用)
// (削除予定) Entityはフラグ管理と削除をGlobalSystemで行うため未使用 (現在未使用)
// (削除予定) Entityはフラグ管理と削除をGlobalSystemで行うため未使用 (現在未使用)
// (削除予定) Entityはフラグ管理と削除をGlobalSystemで行うため未使用 (現在未使用)
namespace Game::Combat::Skill::System
{
	void CleanUpCompletedSkills(eNsECS::EntityMgr& ecs);

	void UpdateSkillLifetimes(eNsECS::EntityMgr& ecs, float deltaTime, gNsSkillData::SkillDatabase& skillDB);

	// スキル状態導入前(最新)
	void updateAttack2DAreaCompLifetimes(eNsECS::EntityMgr& ecs, float deltaTime);
	// 導入後
	// void updateAttack2DAreaCompLifetimes(ECS& ecs);

	// スキル状態導入前
	void updateSkillInstanceCompLifetimes(eNsECS::EntityMgr& ecs, float deltaTime, gNsSkillData::SkillDatabase& skillDB);
	// スキル状態導入後(最新)
	// void updateSkillInstanceCompLifetimes(ECS& ecs);
}