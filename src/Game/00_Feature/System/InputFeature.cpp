#include "InputFeature.h"

#include "Game/Input/SetMapping/SetInputMap.h"
#include "Game/Input/SetMapping/SetSkillInputMap.h"


void Game::Feature::Setting::InputFeature::InitInputMapping(Engine::ECS::EntityMgr& ecs)
{
	Game::Input::Setting::InitInputMap(ecs);
}

void Game::Feature::Setting::InputFeature::InitSkillInputMapping(Engine::ECS::EntityMgr& ecs)
{
	Game::Input::Setting::InitSkillInputMap(ecs);
}