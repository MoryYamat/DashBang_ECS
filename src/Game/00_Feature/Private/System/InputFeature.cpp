#include "InputFeature.h"

#include "Game/Input/Private/SetMapping/SetInputMap.h"
#include "Game/Input/Private/SetMapping/SetSkillInputMap.h"

#include "Engine/InputManager/Public/Types.hpp"
#include "Engine/Graphics/Public/Types.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Input/Public/InputApi.hpp"

void Game::Feature::Setting::InputFeature::InitInputMapping(Engine::ECS::EntityMgr& ecs)
{
	Game::Input::Setting::InitInputMap(ecs);
}

void Game::Feature::Setting::InputFeature::InitSkillInputMapping(Engine::ECS::EntityMgr& ecs)
{
	Game::Input::Setting::InitSkillInputMap(ecs);
}

namespace Game::Feature::Input
{
	void InputFeature::InitInputMapping(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Input::InitAllInputSystem(ctx);
	}

	void InputFeature::InputRoutingSystem(Engine::WorldSystem::Core::WorldCtx& ctx, const Engine::Input::RawInputState& raw)
	{
		Game::Input::InputRouterSystem(ctx, raw);
	}


	void InputFeature::AnalogInputRoutingSystem(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Input::RawInputState& rawInput, const Engine::Graphics::RenderContext& renderContext)
	{
		Game::Input::RouteAnalogInput(ctx, rawInput, renderContext);
	}
}