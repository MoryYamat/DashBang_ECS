#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Feature::Combat
{
	class HitBoxFeature
	{
	public:
		static void InitHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
		static void UpdateHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	private:
	};
}