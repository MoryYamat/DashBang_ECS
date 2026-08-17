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

    template<typename Parameters>
    struct FSMTraits;

    template<typename Instance, typename Parameters>
    void Evaluate(Instance& instance, const Parameters& parameters)
    {
        using Traits = FSMTraits<Parameters>;
        using State = typename Traits::State;

        const auto& definition = FSMTraits<Parameters>::Definition();


        for(const auto& transition : definition.transitions)
        {
            if (static_cast<StateIndex>(instance.current) != transition.source)
            {
                continue;
            }

            const auto& condition = definition.conditions[transition.condition];

            if(condition.evaluate(parameters))
            {
                instance.previous = instance.current;
                instance.current = static_cast<State>(transition.destination);
                ++instance.revision;
                break;
            }
        }
    }
}