#pragma once

#include <ddknd/fsm/runtime/definitions.h>
#include <array>
#include <cstdint>
#include <cassert>

namespace fsm::Nominal
{
    enum class NominalState : std::uint8_t
    {
        A,
        B,
        C,
    };

    enum class NominalParameterIndex : std::uint16_t
    {
        nomParamA = 0,
        nomParamB = 1,
        nomParamC = 2,
    };

    struct NominalParameters
    {
        float nomParamA{};
        int nomParamB{};
        int nomParamC{};
    };

    struct NominalInstance
    {
        NominalState current = NominalState::A;
        NominalState previous = NominalState::A;
        std::uint32_t revision = 0;
    };


    static bool NominalCondition0(const NominalParameters& parameters)
    {
         return parameters.nomParamA <= 10.0;
    }
    static bool NominalCondition1(const NominalParameters& parameters)
    {
         return parameters.nomParamB == 20;
    }
    static bool NominalCondition2(const NominalParameters& parameters)
    {
         return parameters.nomParamB == 20;
    }
    static bool NominalCondition3(const NominalParameters& parameters)
    {
         return parameters.nomParamB == 20 && parameters.nomParamC >= 20;
    }

    using NominalConditionDefinition = ddknd::fsm::ConditionDefinition<NominalParameters>;
    inline constexpr std::array<NominalConditionDefinition, 4>
    NominalConditions = 
    {
        NominalConditionDefinition{&NominalCondition0},
        NominalConditionDefinition{&NominalCondition1},
        NominalConditionDefinition{&NominalCondition2},
        NominalConditionDefinition{&NominalCondition3},
    };

    inline constexpr std::array<ddknd::fsm::TransitionDefinition, 4>
    NominalTransitions = 
    {
        ddknd::fsm::TransitionDefinition{
            .source = static_cast<std::uint32_t>(NominalState::A),
            .destination = static_cast<std::uint32_t>(NominalState::B),
            .condition = static_cast<std::uint32_t>(0),
            .priority = static_cast<std::uint16_t>(120),
            .effect = static_cast<std::uint16_t>(0),
        },
        ddknd::fsm::TransitionDefinition{
            .source = static_cast<std::uint32_t>(NominalState::A),
            .destination = static_cast<std::uint32_t>(NominalState::C),
            .condition = static_cast<std::uint32_t>(1),
            .priority = static_cast<std::uint16_t>(0),
            .effect = static_cast<std::uint16_t>(1),
        },
        ddknd::fsm::TransitionDefinition{
            .source = static_cast<std::uint32_t>(NominalState::B),
            .destination = static_cast<std::uint32_t>(NominalState::A),
            .condition = static_cast<std::uint32_t>(1),
            .priority = static_cast<std::uint16_t>(200),
            .effect = static_cast<std::uint16_t>(2),
        },
        ddknd::fsm::TransitionDefinition{
            .source = static_cast<std::uint32_t>(NominalState::B),
            .destination = static_cast<std::uint32_t>(NominalState::C),
            .condition = static_cast<std::uint32_t>(3),
            .priority = static_cast<std::uint16_t>(10),
            .effect = static_cast<std::uint16_t>(3),
        },
    };

    using NominalDefinition = ddknd::fsm::FSMDefinition<NominalParameters>;
    inline constexpr NominalDefinition NominalDef
    {
        .initialState = static_cast<std::uint32_t>(NominalState::A),
        .conditions = NominalConditions,
        .transitions = NominalTransitions
    };
}

namespace ddknd::fsm
{
    template<>
    struct ddknd::fsm::FSMTraits<::fsm::Nominal::NominalParameters>
    {
        using State = ::fsm::Nominal::NominalState;
        using Instance = ::fsm::Nominal::NominalInstance;

        static constexpr auto& Definition()
        {
              return ::fsm::Nominal::NominalDef;
        };
    };
}