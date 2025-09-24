#pragma once


#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Character/FSM/CC/Effect/StateScoped/Handler/IStateEffectHandler.hpp"

namespace Game::Character::FSM::CC::StateEffect
{
	struct CC2SkillInterferenceHandler :public IStateEffectHandler
	{
		void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e,
			const Game::Character::FSM::CC::StateModel::CCFSMContext& ctx) override
		{

		}
	};
}