#include "CCRequests.hpp"

#include "Game/Character/FSM/CC/StateModel/CCFSMTransitionRequestComponent.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

namespace Game::Character::FSM::CC::API
{
	using namespace Engine::ECS;
	using namespace Game::Character::FSM::CC::StateModel;


	void emitCCRequest
	(
		Engine::ECS::EntityMgr& ecs,
		const Engine::ECS::Entity target,
		const std::type_index to,
		const uint32_t priority,
		const uint32_t causeId
	)
	{
		if (!ecs.hasComponent<CCFSMTransitionRequestComponent>(target))
		{
			Ops::Add<CCFSMTransitionRequestComponent>(ecs, target, CCFSMTransitionRequestComponent{});
			//ecs.addComponent(target, CCFSMTransitionRequestComponent{});
		}
		auto& reqs = ecs.get<CCFSMTransitionRequestComponent>(target);
		reqs.requests.push_back(CCFSMTransitionRequest{ .requestedTo = to, .priority = priority, .causeId = causeId });
	}
}