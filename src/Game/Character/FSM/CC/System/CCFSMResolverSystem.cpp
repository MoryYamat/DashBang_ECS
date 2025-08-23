#include "CCFSMResolverSystem.hpp"

#include "Game/Character/FSM/CC/StateModel/CCStateComponent.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMTransitionRequestComponent.hpp"
#include "Game/Character/FSM/CC/Database/CCFSMDatabase.hpp"

#include <iostream>

namespace Game::Character::FSM::CC::System
{
	using namespace Game::Character::FSM::CC::Database;
	using namespace Game::Character::FSM::CC::StateModel;

	void CCFSMResolverSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
	{
		auto& db = ecs.getResource<CCFSMDatabase>();
		auto& def = db.Get("basic");

		for (auto e : ecs.view<
			CCStateComponent,
			CCFSMTransitionRequestComponent
		>())
		{
			auto& state = ecs.get<CCStateComponent>(e);
			auto& reqs = ecs.get<CCFSMTransitionRequestComponent>(e);


			if (reqs.requests.empty()) continue;

			auto best = std::max_element(
				reqs.requests.begin(),
				reqs.requests.end(),
				[](const auto& a, const auto& b) {
					return a.priority < b.priority;
				}
			);

			state.current = best->requestedTo;

			std::cout << "[CCFSMResolverSystem] Transition applied: "
				<< best->requestedTo.name() << std::endl;


			reqs.requests.clear();
		}

	}
}