#pragma once

#include <ddknd/fsm/axis_registry.h>

#include <vector>

namespace ddknd::fsm
{
    struct AxisBatchCompileResults
    {
        AxisRegistry registry;
        std::vector<CompiledDiagnostic> diagnostics;

        /**
        * Whether All Axes were compiled successfully.
        * Unlike AxisCompileResult::Succeeded(), this is an aggregate over
        * independent per-Axis compilations: apartial success is possible
        * (registry may contain some successfully compiled Axis even when
        * this resturns false). Check `registry` directly if partial results
        * are acceptable for your use case.
        */
        bool Succeeded() const;
        void PrintDiagnostics() const;
    };
} // namespace ddknd::fsm