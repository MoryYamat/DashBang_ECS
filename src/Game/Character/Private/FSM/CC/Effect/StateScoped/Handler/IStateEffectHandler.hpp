#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Game/Character/Private/FSM/CC/StateModel/CCFSMContext.hpp"

namespace Game::Character::FSM::CC::StateEffect
{

	struct IStateEffectHandler
	{
		virtual ~IStateEffectHandler() = default;

		virtual void execute(
			Engine::ECS::EntityMgr&  ecs,
			Engine::ECS::Entity e,
			const Game::Character::FSM::CC::StateModel::CCFSMContext& ctx) = 0;
	};

	// 休憩後 具体的な干渉動作を副作用として実装
	// 休憩後 具体的な干渉動作を副作用として実装
}