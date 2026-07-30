#pragma once

#include <ddknd/fsm/fsm_id.h>

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace ddknd::fsm
{
    enum class CompiledOperator: std::uint8_t
    {
        Greater,
        GreaterEqual,
        Less,
        LessEqual,
        Equal,
        NotEqual,
        AlwaysTrue,
    };

    enum class CompiledValueType : std::uint8_t
    {
        Int,
        Float,
        UInt32,
        Bool
    };

    union RawValue
    {
        int i;
        float f;
        std::uint32_t u;
        bool b;
    };

    struct CompiledValue
    {
        CompiledValueType type = CompiledValueType::Float;
        RawValue raw{};
    };

    struct CompiledOperand
    {
        bool isConstant = false;
        ParameterID parameter{};    // Valid when isConstant is false.
        CompiledValue constant{};   // Valid when isConstant is true.
    };

    struct CompiledCondition
    {
        CompiledOperator op = CompiledOperator::Greater;
        CompiledOperand left;
        CompiledOperand right;
    };

    struct StateTransitionRange
    {
        std::uint32_t begin;
        std::uint32_t count;
    };

    struct CompiledTransitionCondition
    {
        StateID to;
        CompiledCondition condition;
        std::uint8_t priority = 0;
    };

    struct CompiledTransition
    {
        StateID from;
        StateID to;
    };

    struct CompiledFSM
    {
        std::uint32_t ProfileCount = 0;

        /**
         * `transition[].to` is supplementary information. 
         * It's correct to read it from the `transitionConditions`.
         */
        std::vector<CompiledTransition> transitions;

        std::vector<TransitionID> transitionByFrom;

        // index == StateID
        std::vector<StateTransitionRange> stateToTransitionRange;

        // index == transitionID * profileCount + ProfileID
        std::vector<CompiledTransitionCondition> transitionConditions;
    };

    struct CompiledAxis
    {
        std::vector<CompiledFSM> fsms;
        std::uint32_t parameterCount = 0;
    };

    struct CompiledDiagnostic
    {
        std::string message;
    };


    struct AxisCompileResult
    {
        std::optional<CompiledAxis> axis;
        std::vector<CompiledDiagnostic> diagnostics;

        bool Succeeded() const
        {
            return axis.has_value();
        }
    };
} // namespace ddknd::fsm