#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Common/EngineNamespaceDecl.h"
#include "Game/Character/FSM/Movement/StateModel/MovementFSMContext.hpp"

namespace Game::Character::FSM::Movement::Effect
{
	// FIXME: 毎フレーム呼ばれる場合，効率問題が残る
	struct IEffectTemplate
	{
		virtual ~IEffectTemplate() = default;

		virtual void apply(
			eNsECS::EntityMgr& ecs,
			eNsECS::Entity entity,
			const MovementFSMContext& ctx
		) const = 0;
	};
}