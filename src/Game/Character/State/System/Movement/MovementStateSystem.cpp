#include "MovementStateSystem.hpp"
#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/State/Component/MovementStateComponent.hpp"

#include "Game/Character/State/Rule/Movement/MovementStateTransitionDatabase.hpp"
#include "Game/Character/State/Rule/Movement/MovementStateTransitionEvaluator.hpp"



void Game::Character::State::Movement::MovementStateSystem::UpdateStates(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
    auto& db = ecs.getResource<Game::Character::State::Movement::StateTransitionDatabase>();

    for (auto e : ecs.view<Game::Character::State::Movement::CharacterMovementStateComponent, Engine::ECS::Component::Logic2D::Velocity2DComponent>())
    {
        auto& move = ecs.get<Game::Character::State::Movement::CharacterMovementStateComponent>(e);
        auto& vel = ecs.get<Engine::ECS::Component::Logic2D::Velocity2DComponent>(e);

        for (const auto& rule : db.rules)
        {
            // ?
            if (rule.from != move.current)
                continue;

            if (AreConditionsMet(move, vel, deltaTime, rule.conditions))
            {
                std::cout << "[MovementStateSystem(Changed State)] Entity " << e.id
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