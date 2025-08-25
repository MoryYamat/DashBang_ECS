#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Character/FSM/CC/Effect/StateScoped/Hook/CCFSMStateEffectHook.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMDefinition.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMContext.hpp"

#include "Common/EngineNamespaceDecl.h"

#include <typeindex>

namespace Game::Character::FSM::CC::System
{
	using namespace Game::Character::FSM::CC::StateEffect;
	using namespace Game::Character::FSM::CC::StateModel;

	class CCFSMResolverSystem
	{
	public:

		void Update(eNsECS::EntityMgr& ecs, float deltaTime);


	private:

		void tryTriggerEffect(
			eNsECS::EntityMgr& ecs,
			const CCFSMStateEffectHook& hook,
			const eNsECS::Entity entity,
			const CCFSMDefinition& def,
			const CCFSMContext& ctx,
			const std::type_index& current,
			const std::type_index& previous
		);

		void tryTriggerReset(
			eNsECS::EntityMgr& ecs,
			const CCFSMStateEffectHook& hook,
			const eNsECS::Entity entity,
			const CCFSMDefinition& def,
			const CCFSMContext& ctx,
			const std::type_index& current,
			const std::type_index& previous
		);
	};

	// void CCFSMResolverSystem(eNsECS::EntityMgr& ecs, float deltaTime);
}