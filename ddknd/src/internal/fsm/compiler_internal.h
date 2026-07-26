#pragma once

#include <ddknd/fsm/compile_result.h>
#include <ddknd/fsm/axis_definition.h>

namespace ddknd::fsm::internal
{
    bool validateAxis(const AxisDefinition& definition, std::vector<CompiledDiagnostic>& diagnostics);

    bool validateFSM(const AxisDefinition& definition, const FSMDefinition& fsm,
                     std::vector<CompiledDiagnostic>& diagnostics);
    bool validateCondition(const AxisDefinition& axis, const ConditionDefinition& conditionDefinition,
                           std::vector<CompiledDiagnostic>& diagnostics);
    bool validateParameter(const std::size_t parameterSize, const OperandDefinition& operandDefinition,
                           std::vector<CompiledDiagnostic>& diagnostics);
    ValueType resolveOperandType(const AxisDefinition& axis, const OperandDefinition& operandDefinition);
    CompiledFSM compileFSM(const AxisDefinition& axis, FSMDefinition& fsm);

    void sortFSMDefinition(FSMDefinition& fsm);
    void compileTransitionCondition(CompiledTransitionCondition& out, const TransitionConditionDefinition& definition);
    CompiledCondition makeCompiledCondition(const ConditionDefinition& definition);

    ValueType GetValueType(const ConditionValueType& definition);
    CompiledOperator resolveOperator(const ComparisonConditionDefinition& definition);
    CompiledValueType resolveCompiledValueType(const ConditionValueType& definition);
    RawValue makeRawValueFromConditionValueType(const ConditionValueType& definition);
    CompiledOperand makeCompiledOperand(const OperandDefinition& definition);
} // namespace ddknd::fsm::internal