#include "InitializeLayerFeature.h"

#include "Game/00_Feature/Combat/Skill/SkillFeature.h"

#include "Common/GameNamespaceDecl.h"

void Game::Layer::InitializeLayerFeature::DelayedInitialzation(eNsECS::EntityMgr& ecs)
{
	gNsFeature::Combat::SkillFeature::InitializeSkillDatabase(ecs);
}