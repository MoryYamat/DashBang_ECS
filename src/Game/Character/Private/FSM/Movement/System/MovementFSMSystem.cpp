#include "MovementFSMSystem.hpp"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/Private/FSM/Movement/Database/MovementFSMDatabase.hpp"

#include "Game/Character/Private/Control/Movement/Component/Intent/MovementIntentComponent.h"

#include "Game/Character/Private/FSM/Movement/StateModel/MovementFSMTransitionRequestComponent.hpp"
#include "Game/Character/Private/FSM/Movement/StateModel/MovementFSMContext.hpp"

#include "Game/Character/Private/FSM/Movement/StateModel/MovementTransitionTable.hpp"
#include "Game/Character/Private/FSM/Movement/StateModel/MovementStateComponent.hpp"

#include <iostream>

// 
void Game::Character::FSM::Movement::UpdateMovementFSMSystem(Engine::ECS::EntityMgr& ecs)
{
	using namespace Game::Character::Intent;
	using namespace Game::Character::FSM::Movement;
	using namespace Game::Character::FSM::Movement::Database;

	const auto& db = ecs.getResource<MovementFSMDatabase>();
	const auto& def = db.Get("basic"); // TODO: 将来entity事に違う定義を持てるように

	for (Engine::ECS::Entity e : ecs.view<
		MovementStateComponent,
		MovementIntentComponent,
		MovementFSMTransitionRequestComponent>())
	{
		auto& state = ecs.get<MovementStateComponent>(e);
		const auto& intent = ecs.get<MovementIntentComponent>(e);
		auto& reqs = ecs.get<MovementFSMTransitionRequestComponent>(e);

		MovementFSMContext ctx;
		ctx.intentActive = intent.isActive;
		ctx.direction = intent.direction;

		for (const auto& trans : def.transitions)
		{
			if (state.current != trans.from) continue;
			if (!trans.condition->evaluate(ctx)) continue;

			if (reqs.hasExactRequest(trans.to, 0)) continue;

			reqs.requests.push_back(
				{
					.requestedTo = trans.to,
					.priority = 0
				});

			//std::cout << "[MovementFSMResolverSystem] Requesting transition: "
			//	<< trans.from.name() << " -> " << trans.to.name() << std::endl;

			// break; // 一度に複数遷移しない
		}
	}


	//auto& transitions = ecs.getResource<Game::Character::FSM::Movement::MovementTransitionTableResource>().transitions;

	//for (auto entity : ecs.view<MovementStateComponent, Engine::ECS::Component::Logic2D::Velocity2DComponent>())
	//{
	//	auto& state = ecs.get<MovementStateComponent>(entity);
	//	const auto& velocity = ecs.get<Engine::ECS::Component::Logic2D::Velocity2DComponent>(entity).velocity;

	//	MovementFSMContext ctx;
	//	ctx.velocity = velocity;// コンテキストを更新

	//	for (auto& trans : transitions)
	//	{
	//		if (state.current == trans.from && trans.condition->evaluate(ctx))
	//		{
	//			std::cout << "[MovementFSMSystem.cpp]: Transition from "
	//				<< state.current.name() << " to " << trans.to.name() << "\n";

	//			state.current = trans.to; // 状態遷移を実行
	//			break;

	//			// fsm.requestTransition<Movement::Moving>();
	//		}
	//	}
	//}
}