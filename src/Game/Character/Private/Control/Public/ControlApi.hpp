#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Character::Control
{
	void UpdateCharacterMovementIntent(Engine::WorldSystem::Core::WorldCtx& ctx);
	void UpdateCharacterFacingIntent(Engine::WorldSystem::Core::WorldCtx& ctx);


	void UpdateActorPosition(Engine::WorldSystem::Core::WorldCtx& ctx);

}