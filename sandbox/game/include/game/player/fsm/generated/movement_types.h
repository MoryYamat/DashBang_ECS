#pragma once

#include <cstdint>

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

    struct MovementFSMStateComponent
    {
        MovementFSMInstance instance{};
    };

}