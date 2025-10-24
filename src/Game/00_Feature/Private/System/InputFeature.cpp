#include "InputFeature.h"

#include "Game/Input/Private/SetMapping/SetInputMap.h"
#include "Game/Input/Private/SetMapping/SetSkillInputMap.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

void Game::Feature::Setting::InputFeature::InitInputMapping(Engine::ECS::EntityMgr& ecs)
{
	Game::Input::Setting::InitInputMap(ecs);
}

void Game::Feature::Setting::InputFeature::InitSkillInputMapping(Engine::ECS::EntityMgr& ecs)
{
	Game::Input::Setting::InitSkillInputMap(ecs);
}

namespace Game::Feature::Setting
{
	void InputFeature::InitInputMapping(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		
	}

	void InputFeature::InitSkillInputMapping(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}
}