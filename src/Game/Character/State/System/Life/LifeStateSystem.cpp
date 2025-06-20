#include "LifeStateSystem.hpp"

#include "Game/Character/Stats/Component/CharacterStatsComponent.h"
#include "Game/Character/State/Rule/Life/LifeStateTransitionDatabase.hpp"
#include "Game/Character/State/Rule/Life/LifeStateTransitionEvaluator.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Character::State::Life::LifeStateSystem::UpdateStates(eNsECS::EntityMgr& ecs)
{
    auto& db = ecs.getResource<gNsCharaLifeState::StateTransitionDatabase>();

    for (auto e : ecs.view<gNsCharaLifeState::CharacterLifeStateComponent, gNsCharacterStats::CharacterStatsComponent>())
    {
        auto& life = ecs.get<gNsCharaLifeState::CharacterLifeStateComponent>(e);
        auto& stats = ecs.get<gNsCharacterStats::CharacterStatsComponent>(e);

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