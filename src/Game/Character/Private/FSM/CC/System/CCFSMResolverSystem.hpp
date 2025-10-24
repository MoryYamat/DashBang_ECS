#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Character/Private/FSM/CC/Effect/StateScoped/Hook/CCFSMStateEffectHook.hpp"
#include "Game/Character/Private/FSM/CC/StateModel/CCFSMDefinition.hpp"
#include "Game/Character/Private/FSM/CC/StateModel/CCFSMContext.hpp"



#include <typeindex>

namespace Game::Character::FSM::CC::System
{
	
	class CCFSMResolverSystem
	{
	public:

		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);


	private:

		static void tryTriggerEffect(
			Engine::ECS::EntityMgr& ecs,
			const Game::Character::FSM::CC::StateEffect::CCFSMStateEffectHook& hook,
			const Engine::ECS::Entity entity,
			const Game::Character::FSM::CC::StateModel::CCFSMDefinition& def,
			const Game::Character::FSM::CC::StateModel::CCFSMContext& ctx,
			const std::type_index& current,
			const std::type_index& previous
		);

		static void tryTriggerReset(
			Engine::ECS::EntityMgr& ecs,
			const Engine::ECS::Entity entity,
			const Game::Character::FSM::CC::StateModel::CCFSMDefinition& def,
			const std::type_index& current,
			const std::type_index& previous
		);
	};

	// void CCFSMResolverSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);
}