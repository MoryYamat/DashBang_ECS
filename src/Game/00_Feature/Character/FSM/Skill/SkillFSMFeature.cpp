#include "SkillFSMFeature.hpp"

#include "Game/Combat/Skill/FSM/System/SkillFSMSystem.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Feature::Skill::FSM::SkillFSMFeature::UpdateSkillFSMSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsSkillFSM::UpdateSkillFSMSystem(ecs, deltaTime);
}