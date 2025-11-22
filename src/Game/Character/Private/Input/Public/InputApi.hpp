#pragma once

#include "Engine/Public/EngineFwd.hpp"

#include <span>

namespace Game::Character::Input
{
	//void UpdateAnalogInputComp(Engine::WorldSystem::Core::WorldCtx& ctx, std::span<Engine::ECS::Core::Entity> input);

	// init
	void InitInputMapping(Engine::WorldSystem::Core::WorldCtx& ctx);

	// TODO: routing // -> global resource 化?
	void AnalogInputRouting(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Input::RawInputState& rawInput, const Engine::Graphics::RenderContext& renderContext);

	void KeyInputRouting(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Input::RawInputState& rawInput);
}