#include "SkillFeature.h"

#include "Game/Input/Intent/PlayerSkillIntentMapper.h"

#include "Game/Combat/Skill/System/Trigger/PlayerSkillTriggerSystem.h"

#include "Game/Combat/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Skill/System/UpdateSkillPhase.h"// 削除予定：FSM導入後廃止
#include "Game/Combat/Skill/System/SkillTrajectorySystem.h"

#include "Game/Combat/Skill/System/InitializeSkills.h"// 削除予定：FSM導入後廃止

#include "Game/Combat/Skill/FSM/Init/InitSkillDatabase.hpp"// 最新

#include "Game/Combat/Skill/System/Lifetime/SkillExecutionLifetimeSystem.hpp"// 最新

// lifetime
#include "Game/Combat/Skill/System/HitArea/Attack2DAreaLifetimeSystem.hpp"

#include "Game/Combat/Skill/System/Lifetime/HitboxLifetimeControlSystem.hpp"

// lifetimeComponent導入後未使用：削除予定
#include "Game/Combat/Skill/System/Phase/AttackLifetimeSystem.hpp"



// Initialize Database
void Game::Feature::Combat::SkillFeature::InitializeSkillDatabase(eNsECS::EntityMgr& ecs)
{
	// gNsSkillSystem::InitializeSkills(ecs);// 削除予定：FSM導入後廃止
	gNsSkillDatabase::SkillResourceInitialization(ecs);
}

// Intent
void Game::Feature::Combat::SkillFeature::UpateSkillIntent(eNsECS::EntityMgr& ecs)
{
	// std::cout << "here\n";
	gNsInputIntent::SkillIntentMappingSystem::UpdatePlayerSkillIntent(ecs);
}

// Trigger
void Game::Feature::Combat::SkillFeature::TriggerSkillsFromIntent(eNsECS::EntityMgr& ecs)
{
	// 削除予定：廃止 SkillSystemの再構築に伴う
	// player Skill trigger
	// gNsSkillTrigger::PlayerSkillTriggerSystem::TriggerPlayerSkillsFromIntent(ecs);
}

// 廃止
//void Game::Feature::Combat::SkillFeature::SkillGenerateSystem(eNsECS::EntityMgr& ecs)
//{
//	gNsSkillSystem::SpawnSkillHitArea(ecs);
//}


void Game::Feature::Combat::SkillFeature::UpdateSkillTrajectorySystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsSkillSystem::SkillTrajectorySystem::Update(ecs, deltaTime);
}


// 削除予定：SkillExecutionをcharacterアクターへ付与する方式に変更したため
void Game::Feature::Combat::SkillFeature::UpdateSkillExecutionLifetimeSystem(eNsECS::EntityMgr& ecs)
{
	//gNsSkillSystem::UpdateSkillExecutionLifetimeSystem(ecs);
}

void Game::Feature::Combat::SkillFeature::UpdateSkillPhaseSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// 削除予定：FSM導入後廃止
	// gNsSkillSystem::UpdateSkillPhase(ecs, deltaTime);// スキルの段階を更新
	// gNsSkillSystem::UpdateAttack2DAreaLifetimeSystem(ecs, deltaTime);// 攻撃判定のライフタイムを更新

	// lifetimeComponent導入後未使用：削除予定
	// gNsSkillSystem::AttackLifetimeSystem::Update(ecs, deltaTime);// 攻撃判定のライフタイムを更新
}

void Game::Feature::Combat::SkillFeature::UpdateHitoboxLifetimeSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsSkillSystem::HitboxLifetimeControlSystem::Update(ecs, deltaTime);
}