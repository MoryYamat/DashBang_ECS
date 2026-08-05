#pragma once

#include <ddknd/fsm/fsm_id.h>

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <ddknd/math/math.h>

namespace ddknd::fsm
{
    enum class CompiledOperator : std::uint8_t
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
        Bool,
        UVec2,
        UVec3,
        FVec2,
        FVec3
    };

    union RawValue
    {
        int i;
        float f;
        std::uint32_t u;
        bool b;
        ddknd::math::uVec2 uVec2;
        ddknd::math::uVec3 uVec3;
        ddknd::math::Vec2f fVec2;
        ddknd::math::Vec3f fVec3;
    };

    struct CompiledValue
    {
        CompiledValueType type = CompiledValueType::Float;
        RawValue raw{};
    };

    struct CompiledOperand
    {
        bool isConstant = false;
        ParameterID parameter{};  // Valid when isConstant is false.
        CompiledValue constant{}; // Valid when isConstant is true.
    };

    struct CompiledCondition
    {
        CompiledOperator op = CompiledOperator::Greater;
        CompiledValueType operandType = CompiledValueType::Float;// Shared between the left and right sides.
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

        const CompiledFSM& GetFSM(FSMID id) const;
    };

    struct CompiledDiagnostic
    {
        std::string message;

        void PrintMessage() const
        {
            std::cerr << message << "\n";
        }
    };

    struct AxisCompileResult
    {
        std::optional<CompiledAxis> axis;
        std::vector<CompiledDiagnostic> diagnostics;

        /**
         * Whether this single Axis was fully compiled.
         * This is all-or-nothing: if validation fails at any point, `axis` is
         * never populated, so there is no partial/intermediate success state.
         */
        bool Succeeded() const
        {
            return axis.has_value();
        }
    };
} // namespace ddknd::fsm