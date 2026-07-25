#include <ddknd/fsm/compiler.h>

#include <cstddef>
#include <type_traits>
#include <variant>
#include <vector>

namespace ddknd::fsm
{
    AxisCompileResult AxisCompiler::Compile(AxisDefinition& definition)
    {
        AxisCompileResult result{};

        if (!validateAxis(definition, result.diagnostics))
        {
            return result;
        }

        for (const auto& fsm : definition.fsms)
        {
            if (!validateFSM(definition, fsm, result.diagnostics))
            {
                return result;
            }
        }

        CompiledAxis axisResult{};
        for (auto& fsm : definition.fsms)
        {
            axisResult.fsms.emplace_back(compileFSM(definition, fsm));
        }

        result.axis = std::move(axisResult);

        return result;
    }

    bool AxisCompiler::validateAxis(const AxisDefinition& definition, std::vector<CompiledDiagnostic>& diagnostics)
    {
        bool ok = true;

        const auto& fsms = definition.fsms;
        const auto& states = definition.states;
        const auto& parameters = definition.parameters;
        const auto& profiles = definition.profiles;

        if (fsms.size() <= 0)
        {
            diagnostics.push_back(CompiledDiagnostic{.message = "Error: FSMDefinition does not exist."});
            ok = false;
        }

        if (states.size() <= 0)
        {
            diagnostics.push_back(CompiledDiagnostic{.message = "Error: StateDefinition does not exist."});
            ok = false;
        }

        if (profiles.size() <= 0)
        {
            diagnostics.push_back(CompiledDiagnostic{.message = "Error: ProfileDefinition does not exist."});
            ok = false;
        }

        return ok;
    }

    bool AxisCompiler::validateFSM(const AxisDefinition& definition, const FSMDefinition& fsm,
                                   std::vector<CompiledDiagnostic>& diagnostics)
    {
        bool ok = true;

        const auto stateSize = definition.states.size();
        const auto profileSize = definition.profiles.size();
        const auto paremeterSize = definition.parameters.size();
        const auto transitionSize = fsm.transitions.size();

        const auto& transitionDefinitions = fsm.transitions;
        const auto& transitionConditionDefinitions = fsm.transitionConditions;

        for (const auto& transition : transitionDefinitions)
        {
            const auto& name = transition.debugName;
            const StateID fromStateID = transition.from;
            const StateID toStateID = transition.to;

            if (!fromStateID.IsValid())
            {
                diagnostics.emplace_back(CompiledDiagnostic{
                    .message = "Error: The source StateID for the state transition of " + name + " is Invalid."});
                ok = false;
            }
            if (!(static_cast<std::size_t>(fromStateID.Value()) < stateSize))
            {
                diagnostics.emplace_back(CompiledDiagnostic{
                    .message = "Error: The source StateID for the state transition of " + name + " is Invalid."});
                ok = false;
            }

            if (!toStateID.IsValid())
            {
                diagnostics.emplace_back(CompiledDiagnostic{
                    .message = "Error: The destination StateID for the state transition of " + name + " is Invalid."});
                ok = false;
            }
            if (!(static_cast<std::size_t>(toStateID.Value()) < stateSize))
            {
                diagnostics.emplace_back(CompiledDiagnostic{
                    .message = "Error: The destination StateID for the state transition of " + name + " is Invalid."});
                ok = false;
            }
        }

        for (const auto& transitionCondition : transitionConditionDefinitions)
        {
            const TransitionID transitionID = transitionCondition.transition;
            const ProfileID profileID = transitionCondition.profile;
            const auto& condition = transitionCondition.condition;

            if (!transitionID.IsValid())
            {
                diagnostics.emplace_back(CompiledDiagnostic{.message = "Error: Invalid Transition ID."});
                ok = false;
            }
            if (!(static_cast<std::size_t>(transitionID.Value()) < transitionSize))
            {
                diagnostics.emplace_back(CompiledDiagnostic{.message = "Error: Invalid Transition ID."});
                ok = false;
            }

            if (!profileID.IsValid())
            {
                diagnostics.emplace_back(CompiledDiagnostic{.message = "Error: Invalid Profile ID."});
                ok = false;
            }
            if (!(static_cast<std::size_t>(profileID.Value()) < profileSize))
            {
                diagnostics.emplace_back(CompiledDiagnostic{.message = "Error: Invalid Profile ID."});
                ok = false;
            }

            if(!validateCondition(paremeterSize, condition, diagnostics))
            {
                ok = false;
            }
        }

        return ok;
    }

    bool AxisCompiler::validateCondition(const std::size_t parameterSize,
                                         const ConditionDefinition& conditionDefinition,
                                         std::vector<CompiledDiagnostic>& diagnostics)
    {
        return std::visit(
            [parameterSize, &diagnostics](const auto& condition) -> bool
            {
                using T = std::remove_cvref_t<decltype(condition)>;

                if constexpr (std::is_same_v<T, AlwaysTrueConditionDefinition>)
                {
                    return true;
                }
                else if constexpr (std::is_same_v<T, ComparisonConditionDefinition>)
                {
                    const bool leftOk = validateParameter(parameterSize, condition.left, diagnostics);
                    const bool rightOk = validateParameter(parameterSize, condition.right, diagnostics);
                    return leftOk && rightOk;
                }
                else
                {
                    static_assert(std::is_same_v<T, void>, "Unhanded ConditionDefinition type");
                    return false;
                }
            },conditionDefinition);
    }

    bool AxisCompiler::validateParameter(const std::size_t parameterSize, const OperandDefinition& operandDefinition,
                                         std::vector<CompiledDiagnostic>& diagnostics)
    {
        return std::visit(
        [parameterSize, &diagnostics](const auto& operand) -> bool
        {
            using T = std::remove_cvref_t<decltype(operand)>;

            if constexpr (std::is_same_v<T, ConstantOperandDefinition>)
            {
                return true;
            }
            else
            {
                const ParameterID paramID = operand.parameter;

                bool ok = true;
                if(!paramID.IsValid())
                {
                    diagnostics.emplace_back(CompiledDiagnostic{.message = "Error: Invalid Parameter ID."});
                    ok = false;
                }
                if(!(static_cast<std::size_t>(paramID.Value()) < parameterSize))
                {
                    diagnostics.emplace_back(CompiledDiagnostic{.message = "Error: Invalid Parameter ID."});
                    ok = false;
                }

                return ok;
            }
        }, operandDefinition);
    }

    CompiledFSM AxisCompiler::compileFSM(const AxisDefinition& axis, FSMDefinition& fsm)
    {
        CompiledFSM result{};

        return result;
    }
} // namespace ddknd::fsm