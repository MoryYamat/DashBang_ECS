#pragma once

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Game/Character/FSM/Movement/StateModel/MovementFSMContext.hpp"



namespace Game::Character::FSM::Movement::Effect::FSMScoped
{
	using namespace Game::Character::FSM::Movement;

	struct IFSMScopedEffect
	{
		virtual ~IFSMScopedEffect() = default;
		virtual void update(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity entity, const MovementFSMContext& ctx, float deltaTime) const = 0;
	};
}