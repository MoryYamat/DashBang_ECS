#pragma once

#include <cstdint>
#include <iostream>
#include <span>
#include <functional>
#include <vector>

namespace ddknd::fsm
{
    using StateIndex = std::uint32_t;
    using ParameterIndex = std::uint32_t;
    using ConditionIndex = std::uint32_t;
    using EffectIndex = std::uint32_t;

    template<typename Parameters>
    struct ConditionDefinition
    {
        using Predicate = bool (*)(const Parameters&);

        Predicate evaluate;
    };

    struct TransitionDefinition
    {
        StateIndex source;
        StateIndex destination;
        ConditionIndex condition;
        std::uint16_t priority;
        EffectIndex effect;
    };

    template<typename Parameters>
    struct FSMDefinition
    {
        StateIndex initialState;

        std::span<const ConditionDefinition<Parameters>> conditions;
        std::span<const TransitionDefinition> transitions;
    };
}