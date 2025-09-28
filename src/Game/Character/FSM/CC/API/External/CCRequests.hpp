#pragma once


#include "Game/Character/FSM/CC/CCStateTags.hpp"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include <typeindex>
#include <optional>

namespace Game::Character::FSM::CC::API
{
	void emitCCRequest
	(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity target,
		std::type_index to,
		uint32_t priority,
		uint32_t causeId
	);
}