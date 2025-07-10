#include "MovementFSMSystem.hpp"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/FSM/Movement/MovementTransitionTable.hpp"
#include "Game/Character/FSM/Movement/MovementStateComponent.hpp"

#include <iostream>

void Game::Character::FSM::Movement::UpdateMovementFSMSystem(eNsECS::EntityMgr& ecs)
{
	auto& transitions = ecs.getResource<gNsCharaFSMMovement::MovementTransitionTableResource>().transitions;

	for (auto entity : ecs.view<MovementStateComponent, eNsLogic2DComp::Velocity2DComponent>())
	{
		auto& state = ecs.get<MovementStateComponent>(entity);
		const auto& velocity = ecs.get<Engine::ECS::Component::Logic2D::Velocity2DComponent>(entity).velocity;

		MovementContext ctx;
		ctx.velocity = velocity;// コンテキストを更新

		for (auto& trans : transitions)
		{
			if (state.current == trans.from && trans.condition->evaluate(ctx))
			{
				std::cout << "[MovementFSMSystem.cpp]: Transition from "
					<< state.current.name() << " to " << trans.to.name() << "\n";

				state.current = trans.to; // 状態遷移を実行
				break;

				// fsm.requestTransition<Movement::Moving>();
			}
		}
	}
}