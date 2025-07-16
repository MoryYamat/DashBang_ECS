#include "SkillFeature.h"

#include "Game/Input/Intent/PlayerSkillIntentMapper.h"

#include "Game/Combat/Skill/System/Trigger/PlayerSkillTriggerSystem.h"

#include "Game/Combat/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Skill/System/UpdateSkillPhase.h"// íœ—\’èFFSM“±“üŒã”p~
#include "Game/Combat/Skill/System/SkillTrajectorySystem.h"

#include "Game/Combat/Skill/System/InitializeSkills.h"// íœ—\’èFFSM“±“üŒã”p~

#include "Game/Combat/Skill/FSM/InitSkillDatabase.hpp"// ÅV

#include "Game/Combat/Skill/System/SkillExecutionLifetimeSystem.hpp"// ÅV

// lifetime
#include "Game/Combat/Skill/System/HitArea/Attack2DAreaLifetimeSystem.hpp"

// lifetimeComponent“±“üŒã–¢g—pFíœ—\’è
#include "Game/Combat/Skill/System/Phase/AttackLifetimeSystem.hpp"



// Initialize Database
void Game::Feature::Combat::SkillFeature::InitializeSkillDatabase(eNsECS::EntityMgr& ecs)
{
	// gNsSkillSystem::InitializeSkills(ecs);// íœ—\’èFFSM“±“üŒã”p~
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
	// íœ—\’èF”p~ SkillSystem‚ÌÄ\’z‚É”º‚¤
	// player Skill trigger
	// gNsSkillTrigger::PlayerSkillTriggerSystem::TriggerPlayerSkillsFromIntent(ecs);
}

// ”p~
//void Game::Feature::Combat::SkillFeature::SkillGenerateSystem(eNsECS::EntityMgr& ecs)
//{
//	gNsSkillSystem::SpawnSkillHitArea(ecs);
//}


void Game::Feature::Combat::SkillFeature::UpdateSkillTrajectorySystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsSkillSystem::SkillTrajectorySystem::Update(ecs, deltaTime);
}

void Game::Feature::Combat::SkillFeature::UpdateSkillExecutionLifetimeSystem(eNsECS::EntityMgr& ecs)
{
	gNsSkillSystem::UpdateSkillExecutionLifetimeSystem(ecs);
}

void Game::Feature::Combat::SkillFeature::UpdateSkillPhaseSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// íœ—\’èFFSM“±“üŒã”p~
	// gNsSkillSystem::UpdateSkillPhase(ecs, deltaTime);// ƒXƒLƒ‹‚Ì’iŠK‚ğXV
	// gNsSkillSystem::UpdateAttack2DAreaLifetimeSystem(ecs, deltaTime);// UŒ‚”»’è‚Ìƒ‰ƒCƒtƒ^ƒCƒ€‚ğXV

	// lifetimeComponent“±“üŒã–¢g—pFíœ—\’è
	// gNsSkillSystem::AttackLifetimeSystem::Update(ecs, deltaTime);// UŒ‚”»’è‚Ìƒ‰ƒCƒtƒ^ƒCƒ€‚ğXV
}

