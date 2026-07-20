#include "fsm/user_fsm_definition.h"

#include <ddknd/fsm/FSMDefinition.h>

#include <iostream>

namespace fsm::user
{
    void UserFSMDeifinition(ddknd::fsm::FSMDefinitionRegistry& registry)
    {
        using namespace ddknd::fsm;

        AxisDefinition testAxis = {
            .axisName = "test",
            .fsmNames = {"test-fsm"},
            .stateNames = {"first", "second", "third"},
            .conditionNames = {"normal-timer", "AlwaysTrue"},
            .profileNames = {"first-second", "second-third", "third-first"}
        };

        FSMDefinition testFSM = {
            .fsmName = "test-fsm",
            .transitions ={
                TransitionDefinition{
                    .transitionName = "firstTransition",
                    .fromState = "first",
                    .toState = "second"
                },
                TransitionDefinition{
                    .transitionName = "secondTransition",
                    .fromState = "second",
                    .toState = "third"
                },
                TransitionDefinition{
                    .transitionName = "thirdTransition",
                    .fromState = "third",
                    .toState = "first"
                }
            },
            .transitionConditions = {
                TransitionConditionDefinition {
                    .transitionName = "firstTransition",
                    .profileName = "first-second",
                    .condition = ConditionDefinition{
                        .op = Operator::Less,
                        .leftValue = 1.0f
                    },
                    .priority = 1
                },
                TransitionConditionDefinition {
                    .transitionName = "secondTransition",
                    .profileName = "second-third",
                    .condition = ConditionDefinition {
                        .op = Operator::Greater,
                        .rightValue = 1.0f
                    },
                    .priority = 1
                },
                TransitionConditionDefinition {
                    .transitionName = "thirdTransition",
                    .profileName = "third-first",
                    .condition = ConditionDefinition {
                        .op = Operator::Always
                    },
                    .priority = 1
                }
            }
        };

        registry.AddAxisEntry(AxsiRegistryEntry{.axis = testAxis, .fsms = {testFSM}});


        std::cout << "user fsm definition completed\n";
    }
} // namespace fsm::user