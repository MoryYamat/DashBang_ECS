#pragma once

#include "Game/Character/FSM/CC/Reset/Handler/IResetHandler.hpp"

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"



#include "Game/Character/FSM/CC/Effect/StateScoped/CCStateEffectExecutionRecordComponent.hpp"

namespace Game::Character::FSM::CC::Reset
{
	struct ClearEffectExecutionLog : IResetHandler
	{

		void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e
		) const override
		{
			namespace StE = Game::Character::FSM::CC::StateEffect;

			if (ecs.hasComponent<StE::CCStateEffectExecutionRecordComponent>(e))
			{
				auto& record = ecs.get<StE::CCStateEffectExecutionRecordComponent>(e);
				record.clear();
			}
		}
	};
}