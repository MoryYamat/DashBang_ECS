#include "InputFeature.h"

#include "Game/Input/SetMapping/SetInputMap.h"
#include "Game/Input/SetMapping/SetSkillInputMap.h"

#include "Common/GameNamespaceDecl.h"

void Game::Feature::Setting::InputFeature::InitInputMapping(eNsECS::EntityMgr& ecs)
{
	gNsInput::Setting::InitInputMap(ecs);
}

void Game::Feature::Setting::InputFeature::InitSkillInputMapping(eNsECS::EntityMgr& ecs)
{
	gNsInput::Setting::InitSkillInputMap(ecs);
}