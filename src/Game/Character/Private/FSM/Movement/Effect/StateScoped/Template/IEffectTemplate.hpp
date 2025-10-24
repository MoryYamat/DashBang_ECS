#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Character/Private/FSM/Movement/StateModel/MovementFSMContext.hpp"

namespace Game::Character::FSM::Movement::Effect
{
	// FIXME: 毎フレーム呼ばれる場合，効率問題が残る
	struct IEffectTemplate
	{
		virtual ~IEffectTemplate() = default;

		virtual void apply(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity entity,
			const MovementFSMContext& ctx
		) const = 0;
	};
}