#include "CCRequests.hpp"

#include "Game/Character/FSM/CC/StateModel/CCFSMTransitionRequestComponent.hpp"

namespace Game::Character::FSM::CC::API
{
	using namespace Engine::ECS;
	using namespace Game::Character::FSM::CC::StateModel;


	void emitCCRequest
	(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity target,
		std::type_index to,
		uint32_t priority
	)
	{
		if (!ecs.hasComponent<CCFSMTransitionRequestComponent>(target))
		{
			ecs.addComponent(target, CCFSMTransitionRequestComponent{});
		}
		auto& reqs = ecs.get<CCFSMTransitionRequestComponent>(target);
		reqs.requests.push_back(CCFSMTransitionRequest{ .requestedTo = to, .priority = priority});
	}
}