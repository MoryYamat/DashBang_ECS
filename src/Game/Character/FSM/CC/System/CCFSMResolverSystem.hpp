#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Character/FSM/CC/Effect/StateScoped/Hook/CCFSMStateEffectHook.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMDefinition.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMContext.hpp"



#include <typeindex>

namespace Game::Character::FSM::CC::System
{
	using namespace Game::Character::FSM::CC::StateEffect;
	using namespace Game::Character::FSM::CC::StateModel;

	class CCFSMResolverSystem
	{
	public:

		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);


	private:

		static void tryTriggerEffect(
			Engine::ECS::EntityMgr& ecs,
			const CCFSMStateEffectHook& hook,
			const Engine::ECS::Entity entity,
			const CCFSMDefinition& def,
			const CCFSMContext& ctx,
			const std::type_index& current,
			const std::type_index& previous
		);

		static void tryTriggerReset(
			Engine::ECS::EntityMgr& ecs,
			const Engine::ECS::Entity entity,
			const CCFSMDefinition& def,
			const std::type_index& current,
			const std::type_index& previous
		);
	};

	// void CCFSMResolverSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);
}