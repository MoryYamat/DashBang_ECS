#include "SkillFeature.h"

#include "Game/Input/Intent/PlayerSkillIntentMapper.h"

#include "Game/Combat/Skill/Trigger/PlayerSkillTriggerSystem.h"

#include "Game/Combat/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Skill/System/UpdateSkillPhase.h"
#include "Game/Combat/Skill/System/SkillTrajectorySystem.h"

#include "Game/Combat/Skill/System/InitializeSkills.h"

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
	gNsSkillTrigger::PlayerSkillTriggerSystem::TriggerPlayerSkillsFromIntent(ecs);
}

// ”pŽ~
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
	gNsSkillSystem::UpdateSkillPhase(ecs, deltaTime);
}