#include "ActionStateSystem.hpp"

#include "Game/Character/State/System/Action/ActionStateSystem.hpp"
#include "Game/Character/State/Component/Action/ActionStateComponent.hpp"
#include "Game/Character/State/Rule/Action/ActionStateTransitionDatabase.hpp"

#include "Game/Character/State/Component/Action/Skill/CharacterSkillExecutionStateComponent.hpp"

#include "Game/Character/Control/Skill/Intent/SkillIntentComponent.hpp"

#include "Game/Character/State/Rule/Action/ActionStateTransitionEvaluator.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Character::State::Action::ActionStateSystem::UpdateStates(eNsECS::EntityMgr& ecs, float deltaTime)
{
    auto& db = ecs.getResource <gNsCharaActionState::StateTransitionDatabase>();

    //for (auto e : ecs.view<gNsCharaActionState::CharacterActionStateComponent,
    //    gNsCharacterControlSkill::SkillIntentComponent>())
	for (eNsECS::Entity e : ecs.view<gNsCharaActionState::CharacterActionStateComponent,
		gNsCharaActionState::CharacterSkillExecutionStateComponent>())
    {
        auto& action = ecs.get<gNsCharaActionState::CharacterActionStateComponent>(e);
        auto& skillExec = ecs.get<gNsCharaActionState::CharacterSkillExecutionStateComponent>(e);

        for (const auto& rule : db.rules)
        {
            // ?
            if (rule.from != action.current)
                continue;

            if (gNsCharaActionState::AreConditionsMet(action, skillExec, deltaTime, rule.conditions))
            {
                std::cout << "[ActionStateSystem(Changed State)] Entity " << e.id
                    << ": " << gNsCharaActionState::ToString(action.current)
                    << " -> " << gNsCharaActionState::ToString(rule.to) << std::endl;



                action.current = rule.to;
                break;
            }

        }


        //std::cout << "[LifeStateSystem(NowState)] Entity " << e.id
        //    << ": " << ToString(life.current) << std::endl;
    }
}