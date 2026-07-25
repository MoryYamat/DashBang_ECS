#pragma once

#include <ddknd/fsm/compile_result.h>
#include <ddknd/fsm/fsm_definition.h>

#include <cstddef>
#include <vector>

namespace ddknd::fsm
{
    class AxisCompiler
    {
      public:
        static AxisCompileResult Compile(AxisDefinition& definition);

      private:
        static bool validateAxis(const AxisDefinition& definition, std::vector<CompiledDiagnostic>& diagnostics);

        static bool validateFSM(const AxisDefinition& definition, const FSMDefinition& fsm,
                                std::vector<CompiledDiagnostic>& diagnostics);

        static bool validateCondition(const std::size_t parameterSize, const ConditionDefinition& conditionDefinition,
                                      std::vector<CompiledDiagnostic>& diagnostics);

        static bool validateParameter(const std::size_t parameterSize, const OperandDefinition& operandDefinition, std::vector<CompiledDiagnostic>& diagnostics);
        static CompiledFSM compileFSM(const AxisDefinition& axis, FSMDefinition& fsm);
    };
} // namespace ddknd::fsm