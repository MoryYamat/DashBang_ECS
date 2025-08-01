#pragma once

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Game/Character/FSM/Movement/StateModel/MovementFSMContext.hpp"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::FSM::Movement::Effect::FSMScoped
{
	using namespace Game::Character::FSM::Movement;

	struct IFSMScopedEffect
	{
		virtual ~IFSMScopedEffect() = default;
		virtual void update(eNsECS::EntityMgr& ecs, eNsECS::Entity entity, const MovementFSMContext& ctx, float deltaTime) const = 0;
	};
}