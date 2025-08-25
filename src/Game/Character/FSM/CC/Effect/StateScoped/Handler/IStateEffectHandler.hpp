#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Game/Character/FSM/CC/StateModel/CCFSMContext.hpp"

namespace Game::Character::FSM::CC::StateEffect
{
	using namespace Engine::ECS;
	using namespace Game::Character::FSM::CC::StateModel;

	struct IStateEffectHandler
	{
		virtual ~IStateEffectHandler() = default;

		virtual void execute(
			Entity e, 
			const CCFSMContext& ctx) = 0;
	};
}