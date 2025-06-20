#include "MovementStateSystem.hpp"
#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/State/Component/MovementStateComponent.hpp"

#include "Game/Character/State/Rule/Movement/MovementStateTransitionDatabase.hpp"
#include "Game/Character/State/Rule/Movement/MovementStateTransitionEvaluator.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Character::State::Movement::MovementStateSystem::UpdateStates(eNsECS::EntityMgr& ecs, float deltaTime)
{
    auto& db = ecs.getResource<gNsCharaMoveState::StateTransitionDatabase>();

    for (auto e : ecs.view<gNsCharaMoveState::CharacterMovementStateComponent, eNsLogic2DComp::Velocity2DComponent>())
    {
        auto& move = ecs.get<gNsCharaMoveState::CharacterMovementStateComponent>(e);
        auto& vel = ecs.get<eNsLogic2DComp::Velocity2DComponent>(e);

        for (const auto& rule : db.rules)
        {
            // ?
            if (rule.from != move.current)
                continue;

            if (AreConditionsMet(move, vel, deltaTime, rule.conditions))
            {
                std::cout << "[LifeStateSystem(Changed State)] Entity " << e.id
                    << ": " << ToString(move.current)
                    << " -> " << ToString(rule.to) << std::endl;



                move.current = rule.to;
                break;
            }

        }


        //std::cout << "[LifeStateSystem(NowState)] Entity " << e.id
        //    << ": " << ToString(life.current) << std::endl;
    }
}