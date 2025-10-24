#include "ActionStateSystem.hpp"

#include "Game/Character/Private/State/System/Action/ActionStateSystem.hpp"
#include "Game/Character/Private/State/Component/Action/ActionStateComponent.hpp"
#include "Game/Character/Private/State/Rule/Action/ActionStateTransitionDatabase.hpp"

#include "Game/Character/Private/State/Component/Action/Skill/CharacterSkillExecutionStateComponent.hpp"

#include "Game/Character/Private/Control/Skill/Intent/SkillIntentComponent.hpp"

#include "Game/Character/Private/State/Rule/Action/ActionStateTransitionEvaluator.hpp"



void Game::Character::State::Action::ActionStateSystem::UpdateStates(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
    auto& db = ecs.getResource <Game::Character::State::Action::StateTransitionDatabase>();

    //for (auto e : ecs.view<Game::Character::State::Action::CharacterActionStateComponent,
    //    Game::Character::Control::Skill::SkillIntentComponent>())
	for (Engine::ECS::Entity e : ecs.view<Game::Character::State::Action::CharacterActionStateComponent,
		Game::Character::State::Action::CharacterSkillExecutionStateComponent>())
    {
        auto& action = ecs.get<Game::Character::State::Action::CharacterActionStateComponent>(e);
        auto& skillExec = ecs.get<Game::Character::State::Action::CharacterSkillExecutionStateComponent>(e);

        for (const auto& rule : db.rules)
        {
            // ?
            if (rule.from != action.current)
                continue;

            if (Game::Character::State::Action::AreConditionsMet(action, skillExec, deltaTime, rule.conditions))
            {
                std::cout << "[ActionStateSystem(Changed State)] Entity " << e.id
                    << ": " << Game::Character::State::Action::ToString(action.current)
                    << " -> " << Game::Character::State::Action::ToString(rule.to) << std::endl;



                action.current = rule.to;
                break;
            }

        }


        //std::cout << "[LifeStateSystem(NowState)] Entity " << e.id
        //    << ": " << ToString(life.current) << std::endl;
    }
}