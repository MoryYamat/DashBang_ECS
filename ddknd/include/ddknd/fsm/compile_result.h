#pragma once

#include <ddknd/fsm/fsm_id.h>

#include <optional>
#include <string>
#include <vector>

namespace ddknd::fsm
{
    struct CompiledCondition
    {

    };

    struct CompiledTransitionCondition
    {
        CompiledCondition condition;
        std::uint8_t priority = 0;
    };

    /**
    * the range of transition indices.
    */
    struct CompiledTransitionRange
    {
        std::size_t begin;
        std::size_t end;
    };

    struct CompiledFSM
    {
        std::uint32_t ProfileCount = 0;
        
        // index == from
        std::vector<CompiledTransitionRange> transitions;
        // index == transitionID * profileCount + ProfileID
        std::vector<CompiledTransitionCondition> transitionConditions;
    };

    struct CompiledAxis
    {
        std::vector<CompiledFSM> fsms;
    };

    struct CompiledDiagnostic
    {
        std::string message;
    };

    struct AxisCompileResult
    {
        std::optional<CompiledAxis> axis;
        std::vector<CompiledDiagnostic> diagnostics;

        bool Succeeded()const
        {
            return axis.has_value();
        }
    };
}