#include "SkillFSMFeature.hpp"

#include "Game/Combat/Skill/FSM/System/SkillFSMSystem.hpp"
#include "Game/Combat/Skill/FSM/System/SkillFSMResolverSystem.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Feature::Skill::FSM::SkillFSMFeature::UpdateSkillFSMSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// transition request ëóêM
	gNsSkillFSM::UpdateSkillFSMSystem(ecs, deltaTime);

}

// transition request resolver
void Game::Feature::Skill::FSM::SkillFSMFeature::UpdateSkillFSMResolverSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsSkillFSM::System::SkillFSMResolverSystem::Update(ecs, deltaTime);
}
