#include "SkillFeature.h"

#include "Game/Input/Intent/PlayerSkillIntentMapper.h"

#include "Game/Combat/Skill/System/Trigger/PlayerSkillTriggerSystem.h"

#include "Game/Combat/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Skill/System/UpdateSkillPhase.h"
#include "Game/Combat/Skill/System/SkillTrajectorySystem.h"

#include "Game/Combat/Skill/System/InitializeSkills.h"

// lifetime
#include "Game/Combat/Skill/System/HitArea/Attack2DAreaLifetimeSystem.hpp"

// lifetimeComponent導入後未使用：削除予定
#include "Game/Combat/Skill/System/Phase/AttackLifetimeSystem.hpp"



// Initialize Database
void Game::Feature::Combat::SkillFeature::InitializeSkillDatabase(eNsECS::EntityMgr& ecs)
{
	gNsSkillSystem::InitializeSkills(ecs);
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



void Game::Feature::Combat::SkillFeature::UpdateSkillPhaseSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsSkillSystem::UpdateSkillPhase(ecs, deltaTime);// スキルの段階を更新
	// gNsSkillSystem::UpdateAttack2DAreaLifetimeSystem(ecs, deltaTime);// 攻撃判定のライフタイムを更新

	// lifetimeComponent導入後未使用：削除予定
	// gNsSkillSystem::AttackLifetimeSystem::Update(ecs, deltaTime);// 攻撃判定のライフタイムを更新
}

