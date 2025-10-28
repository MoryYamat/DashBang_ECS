#pragma once

#include "Engine/Public/EngineFwd.hpp"
#include <span>

namespace Game::Character::FSM::Movement
{



	void MovementTransitionResolver(Engine::WorldSystem::Core::WorldCtx& ctx
		, std::span<Engine::ECS::Core::Entity> mover);
}