#include "LifeStateSystem.hpp"

#include "Game/Character/Stats/Public/StatsComponent.hpp"
#include "Game/Character/Private/State/Rule/Life/LifeStateTransitionDatabase.hpp"
#include "Game/Character/Private/State/Rule/Life/LifeStateTransitionEvaluator.hpp"




void Game::Character::State::Life::LifeStateSystem::UpdateStates(Engine::ECS::EntityMgr& ecs)
{
    auto& db = ecs.getResource<Game::Character::State::Life::StateTransitionDatabase>();

    for (auto e : ecs.view<Game::Character::State::Life::CharacterLifeStateComponent, Game::Character::Stats::CharacterStatsComponent>())
    {
        auto& life = ecs.get<Game::Character::State::Life::CharacterLifeStateComponent>(e);
        auto& stats = ecs.get<Game::Character::Stats::CharacterStatsComponent>(e);

        for (const auto& rule : db.rules)
        {
            // ?
            if (rule.from != life.current)
                continue;

            if (AreConditionsMet(life, stats, rule.conditions))
            {
                std::cout << "[LifeStateSystem(Changed State)] Entity " << e.id
                    << ": " << ToString(life.current)
                    << " -> " << ToString(rule.to) << std::endl;



                life.current = rule.to;
                break;
            }

        }


        //std::cout << "[LifeStateSystem(NowState)] Entity " << e.id
        //    << ": " << ToString(life.current) << std::endl;
    }
}