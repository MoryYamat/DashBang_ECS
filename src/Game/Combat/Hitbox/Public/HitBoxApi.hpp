#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Game/Combat/HitBox/Public/HitBoxFwd.hpp"

namespace Game::Combat::HitBox
{
	// private
	void RegisterHitBoxDTO(HitBoxBuilder& builder);


	// public
	void InitAllHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	void UpdateAllHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
}