#include "SkillFeature.h"

#include "Game/Input/Intent/PlayerSkillIntentMapper.h"

#include "Game/Combat/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Skill/System/UpdateSkillPhase.h"
#include "Game/Combat/Skill/System/SkillTrajectorySystem.h"

void Game::Feature::SkillFeature::InitializeSkillDatabase(eNsECS::EntityMgr& ecs)
{
	gNsSkillSystem::InitializeSkills(eNsECS::EntityMgr & ecs);
}

void Game::Feature::SkillFeature::UpateSkillIntent(eNsECS::EntityMgr& ecs)
{
	gNsInputIntent::SkillIntentMappingSystem::UpdatePlayerSkillIntent(ecs);
}


void Game::Feature::SkillFeature::SkillGenerateSystem(eNsECS::EntityMgr& ecs, gNsSkillData::SkillDatabase& skillDB)
{
	gNsSkillSystem::SpawnSkillHitArea(ecs, skillDB);
}


void Game::Feature::SkillFeature::UpdateSkillTrajectorySystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsSkillSystem::SkillTrajectorySystem::Update(ecs, deltaTime);
}


void Game::Feature::SkillFeature::UpdateSkillPhaseSystem(eNsECS::EntityMgr& ecs, float deltaTime, gNsSkillData::SkillDatabase& skillDB)
{
	gNsSkillSystem::UpdateSkillPhase(ecs, deltaTime, skillDB);

}