#pragma once

#include <ddknd/fsm/runtime/definitions.h>
#include <array>
#include <cstdint>
#include <cassert>

namespace fsm::MovementFSM
{
    enum class MovementFSMState : std::uint8_t
    {
        Idle,
        Run,
    };

    enum class MovementFSMParameterIndex : std::uint16_t
    {
        MovementIntent = 0,
    };

    struct MovementFSMParameters
    {
        float MovementIntent{};
    };

    struct MovementFSMInstance
    {
        MovementFSMState current = MovementFSMState::Idle;
        MovementFSMState previous = MovementFSMState::Idle;
        std::uint32_t revision = 0;
    };

    // I added manually.
    struct MovementFSMStateComponent
    {
        MovementFSMInstance instance{};
    };
    
    static bool MovementFSMCondition0(const MovementFSMParameters& parameters)
    {
         return parameters.MovementIntent > 1e-05;
    }
    static bool MovementFSMCondition1(const MovementFSMParameters& parameters)
    {
         return parameters.MovementIntent == 0.0;
    }

    using MovementFSMConditionDefinition = ddknd::fsm::ConditionDefinition<MovementFSMParameters>;
    inline constexpr std::array<MovementFSMConditionDefinition, 2>
    MovementFSMConditions = 
    {
        MovementFSMConditionDefinition{&MovementFSMCondition0},
        MovementFSMConditionDefinition{&MovementFSMCondition1},
    };

    inline constexpr std::array<ddknd::fsm::TransitionDefinition, 2>
    MovementFSMTransitions = 
    {
        ddknd::fsm::TransitionDefinition{
            .source = static_cast<std::uint32_t>(MovementFSMState::Idle),
            .destination = static_cast<std::uint32_t>(MovementFSMState::Run),
            .condition = static_cast<std::uint32_t>(0),
            .priority = static_cast<std::uint16_t>(100),
            .effect = static_cast<std::uint16_t>(0),
        },
        ddknd::fsm::TransitionDefinition{
            .source = static_cast<std::uint32_t>(MovementFSMState::Run),
            .destination = static_cast<std::uint32_t>(MovementFSMState::Idle),
            .condition = static_cast<std::uint32_t>(1),
            .priority = static_cast<std::uint16_t>(100),
            .effect = static_cast<std::uint16_t>(1),
        },
    };

    using MovementFSMDefinition = ddknd::fsm::FSMDefinition<MovementFSMParameters>;
    inline constexpr MovementFSMDefinition MovementFSMDef
    {
        .initialState = static_cast<std::uint32_t>(MovementFSMState::Idle),
        .conditions = MovementFSMConditions,
        .transitions = MovementFSMTransitions
    };
}

namespace ddknd::fsm
{
    template<>
    struct ddknd::fsm::FSMTraits<::fsm::MovementFSM::MovementFSMParameters>
    {
        using State = ::fsm::MovementFSM::MovementFSMState;
        using Instance = ::fsm::MovementFSM::MovementFSMInstance;

        static constexpr auto& Definition()
        {
              return ::fsm::MovementFSM::MovementFSMDef;
        };
    };
}