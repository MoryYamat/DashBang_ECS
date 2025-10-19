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
void Game::Feature::Combat::SkillFeature::InitializeSkillDatabase(Engine::ECS::EntityMgr& ecs)
{
	// Game::Combat::Skill::System::InitializeSkills(ecs);// 削除予定：FSM導入後廃止
	Game::Combat::Skill::Database::SkillResourceInitialization(ecs);
}

// Intent
void Game::Feature::Combat::SkillFeature::UpateSkillIntent(Engine::ECS::EntityMgr& ecs)
{
	// std::cout << "here\n";
	Game::Input::Intent::SkillIntentMappingSystem::UpdatePlayerSkillIntent(ecs);
}

// Trigger
void Game::Feature::Combat::SkillFeature::TriggerSkillsFromIntent(Engine::ECS::EntityMgr& ecs)
{
	// 削除予定：廃止 SkillSystemの再構築に伴う
	// player Skill trigger
	// Game::Combat::SkillTrigger::PlayerSkillTriggerSystem::TriggerPlayerSkillsFromIntent(ecs);
}

// 廃止
//void Game::Feature::Combat::SkillFeature::SkillGenerateSystem(Engine::ECS::EntityMgr& ecs)
//{
//	Game::Combat::Skill::System::SpawnSkillHitArea(ecs);
//}


void Game::Feature::Combat::SkillFeature::UpdateSkillTrajectorySystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	Game::Combat::Skill::System::SkillTrajectorySystem::Update(ecs, deltaTime);
}


// 削除予定：SkillExecutionをcharacterアクターへ付与する方式に変更したため
void Game::Feature::Combat::SkillFeature::UpdateSkillExecutionLifetimeSystem(Engine::ECS::EntityMgr& ecs)
{
	//Game::Combat::Skill::System::UpdateSkillExecutionLifetimeSystem(ecs);
}

void Game::Feature::Combat::SkillFeature::UpdateSkillPhaseSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	// 削除予定：FSM導入後廃止
	// Game::Combat::Skill::System::UpdateSkillPhase(ecs, deltaTime);// スキルの段階を更新
	// Game::Combat::Skill::System::UpdateAttack2DAreaLifetimeSystem(ecs, deltaTime);// 攻撃判定のライフタイムを更新

	// lifetimeComponent導入後未使用：削除予定
	// Game::Combat::Skill::System::AttackLifetimeSystem::Update(ecs, deltaTime);// 攻撃判定のライフタイムを更新
}

void Game::Feature::Combat::SkillFeature::UpdateHitoboxLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	Game::Combat::Skill::System::HitboxLifetimeControlSystem::Update(ecs, deltaTime);
}
