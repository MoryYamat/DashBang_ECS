// #include <ddknd/fsm/runtime/definitions.h>

#include "generated/nominal_generated.h"

#include <iostream>

#include <cassert>

int main()
{
    NominalParameters nom_parameters{};
    NominalInstance stateInst{};

    // accessors are called by the engine side evaluater.
    // API for pass parameters value to the engine, is like below.
    // evaluator is set of pure functions. It doesn't have state internally. Evaluate settings are provided from stateInstances and parameterInstances. 
    // next_state = ddknd::fsm::Evaluator(stateInst, nom_parameters);

    // PreComputedRuntimeDefinitions
    // - flattened array or CSR which 

    // Informations required to definine conditions
    // - All ParameterIndices to computing whether transition occur or not.
    // - Operator for reconstructing the condition expressions.
    // - Literals for reconstructing the condition expressions.

    struct Temp{};

    // The specifications which Temp must have.
    // - It can represent Literal / ParameterIndex / ConditionDefinition
    // - 


    // how to reconstruct condition expressions from the definition in runtime.
    using ConditionIndex = std::uint32_t;
    using EffectIndex = std::uint32_t;

    // evaluator calls accessors internally and get the value to evaluate the conditions are fullfilled or not .
    struct TransitionDefinition
    {
        NominalState to = NominalState::B;
        ConditionIndex condition_index = 0;
        std::uint8_t priority = 120;
        EffectIndex effect = 0;
    };

    constexpr ddknd::fsm::FSMDefinition<NominalParameters>
    NominalDefinition
    {
        .initialState = 0,
        .conditions = NominalConditions,
        .transitions = {}
    };
    std::cout << "here\n";
    auto result = NominalDefinition.conditions[0].evaluate(nom_parameters);
    std::cout << "result = " << result << "\n";

    nom_parameters.nomParamA = 30;

    auto result2 = NominalDefinition.conditions[0].evaluate(nom_parameters);
    std::cout << "result = " << result2 << "\n";

    // most simple method to reconstructing condition expression.

    return 0;
}

// #pragma once

// #include <ddknd/fsm/runtime/definitions.h>

// #include <cstdint>
// #include <cassert>
// #include <exception>

// enum class NominalState : std::uint8_t
// {
//      A,
//      B,
//      C,
// };
// enum class NominalParameterIndex : std::uint16_t
// {
//      nomParamA = 0,
//      nomParamB = 1,
//      nomParamC = 2,
// };
// struct NominalParameters
// {
//       float nomParamA{};
//       int nomParamB{};
//       int nomParamC{};
// };
// struct NominalInstance
// {
//       NominalState current = NominalState::A;
//       NominalState previous = NominalState::A;
//       std::uint32_t revision = 0;
// };

// static bool NominalCondition0(const NominalParameters& parameters)
// {
//       return parameters.nomParamA > 0.5f;
// }

// static bool NominalCondition1(const NominalParameters& parameters)
// {
//       return parameters.nomParamB == 10;
// }

// inline constexpr ddknd::fsm::ConditionDefinition<NominalParameters>
// NominalConditions[] = 
// {
//       {&NominalCondition0},
//       {&NominalCondition1}
// };

// inline constexpr ddknd::fsm::TransitionDefinition
// NominalTransitions[] = 
// {
//       {
//             .source = static_cast<std::uint32_t>(NominalState::A),
//             .destination = static_cast<std::uint32_t>(NominalState::B),
//             .condition = 0,
//             .priority = 120,
//             .effect = 0
//       },
// };

// inline constexpr ddknd::fsm::FSMDefinition<NominalParameters>
// NominalDefinition
// {
//       .initialState = static_cast<std::uint32_t>(NominalState::A),
//       .conditions = NominalConditions,
//       .transitions = NominalTransitions
// };