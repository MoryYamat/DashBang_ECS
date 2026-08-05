#include "internal/fsm/compiler_internal.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include <ddknd/math/math.h>

namespace ddknd::fsm::internal
{

    bool validateAxis(const AxisDefinition& definition, std::vector<CompiledDiagnostic>& diagnostics)
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

    bool validateFSM(const AxisDefinition& definition, const FSMDefinition& fsm,
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

            if (!validateCondition(definition, condition, diagnostics))
            {
                ok = false;
            }
        }

        return ok;
    }

    bool validateCondition(const AxisDefinition& axis, const ConditionDefinition& conditionDefinition,
                           std::vector<CompiledDiagnostic>& diagnostics)
    {
        return std::visit(
            [&](const auto& condition) -> bool
            {
                using T = std::remove_cvref_t<decltype(condition)>;

                if constexpr (std::is_same_v<T, AlwaysTrueConditionDefinition>)
                {
                    return true;
                }
                else if constexpr (std::is_same_v<T, ComparisonConditionDefinition>)
                {
                    bool ok = validateParameter(axis.parameters.size(), condition.left, diagnostics) &&
                              validateParameter(axis.parameters.size(), condition.right, diagnostics);

                    if (ok)
                    {
                        const auto leftType = resolveOperandType(axis, condition.left);
                        const auto rightType = resolveOperandType(axis, condition.right);
                        if (leftType != rightType)
                        {
                            diagnostics.push_back(CompiledDiagnostic{
                                .message = "Error: left/right operand type mismatch in ComparisonCondition."});
                            ok = false;
                        }
                    }
                    return ok;
                }
                else
                {
                    static_assert(std::is_same_v<T, void>, "Unhanded ConditionDefinition type");
                    return false;
                }
            },
            conditionDefinition);
    }

    bool validateParameter(const std::size_t parameterSize, const OperandDefinition& operandDefinition,
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
                    if (!paramID.IsValid())
                    {
                        diagnostics.emplace_back(CompiledDiagnostic{.message = "Error: Invalid Parameter ID."});
                        ok = false;
                    }
                    if (!(static_cast<std::size_t>(paramID.Value()) < parameterSize))
                    {
                        diagnostics.emplace_back(CompiledDiagnostic{.message = "Error: Invalid Parameter ID."});
                        ok = false;
                    }

                    return ok;
                }
            },
            operandDefinition);
    }

    ValueType resolveOperandType(const AxisDefinition& axis, const OperandDefinition& operandDefinition)
    {
        return std::visit(
            [&axis](auto&& x) -> ValueType
            {
                using T = std::remove_cvref_t<decltype(x)>;

                if constexpr (std::is_same_v<T, ConstantOperandDefinition>)
                {
                    return internal::GetValueType(x.value);
                }
                else
                {
                    return axis.parameters[x.parameter.Value()].type;
                }
            },
            operandDefinition);
    }

    CompiledFSM compileFSM(const AxisDefinition& axis, FSMDefinition& fsm)
    {
        CompiledFSM result{};
        const auto ProfileCount = axis.profiles.size();
        const auto TransitionCount = fsm.transitions.size();

        result.transitions.resize(TransitionCount);
        result.transitionConditions.resize(ProfileCount * TransitionCount);

        result.ProfileCount = static_cast<std::uint32_t>(ProfileCount);

        for (auto i = 0; i < fsm.transitions.size(); i++)
        {
            result.transitions[i].from = fsm.transitions[i].from;
            result.transitions[i].to = fsm.transitions[i].to;
        }

        std::vector<std::pair<StateID, TransitionID>> pairs;
        for (auto i = 0; i < fsm.transitions.size(); i++)
        {
            pairs.push_back({fsm.transitions[i].from, TransitionID{static_cast<std::uint32_t>(i)}});
        }
        std::sort(pairs.begin(), pairs.end(), [](auto& a, auto& b) { return a.first.Value() < b.first.Value(); });

        result.transitionByFrom.reserve(pairs.size());
        for (auto& [from, id] : pairs)
        {
            result.transitionByFrom.push_back(id);
        }

        result.stateToTransitionRange.assign(axis.states.size(), StateTransitionRange{0, 0});
        std::size_t i = 0;
        while (i < pairs.size())
        {
            const auto stateValue = pairs[i].first.Value();
            const auto begin = i;
            while (i < pairs.size() && pairs[i].first.Value() == stateValue)
            {
                i++;
            }
            result.stateToTransitionRange[stateValue] = StateTransitionRange{
                .begin = static_cast<std::uint32_t>(begin), .count = static_cast<std::uint32_t>(i - begin)};
        }

        for (auto i = 0; i < fsm.transitionConditions.size(); i++)
        {
            auto transitionValue = fsm.transitionConditions[i].transition.Value();
            auto profileValue = fsm.transitionConditions[i].profile.Value();
            auto priority = fsm.transitionConditions[i].priority;

            auto index = transitionValue * ProfileCount + profileValue;

            result.transitionConditions[index].to = fsm.transitions[transitionValue].to;
            result.transitionConditions[index].priority = fsm.transitionConditions[i].priority;
            result.transitionConditions[index].condition = makeCompiledCondition(axis, fsm.transitionConditions[i].condition);
        }

        return result;
    }

    void sortFSMDefinition(FSMDefinition& fsm)
    {
        auto& transitions = fsm.transitions;
        auto& transitionConditions = fsm.transitionConditions;

        std::sort(transitionConditions.begin(), transitionConditions.end(),
                  [](auto& a, auto& b)
                  {
                      if (a.transition != b.transition)
                      {
                          return a.transition.Value() < b.transition.Value();
                      }
                      else if (a.profile != b.profile)
                      {
                          return a.profile.Value() < b.profile.Value();
                      }
                      else // This must be not happen, I think.
                          return a.priority > b.priority;
                  });

        // for(const auto& tc : transitionConditions)
        // {
        //     std::cout << "transition=" << tc.transition.Value()
        //               << " profile=" << tc.profile.Value()
        //               << " priority=" << static_cast<std::uint32_t>(tc.priority) << "\n";
        // }
    }

    CompiledCondition makeCompiledCondition(const AxisDefinition& axis, const ConditionDefinition& definition)
    {
        return std::visit(
            [&](const auto& x) -> CompiledCondition
            {
                using T = std::remove_cvref_t<decltype(x)>;
                if constexpr (std::is_same_v<T, AlwaysTrueConditionDefinition>)
                {
                    return CompiledCondition{.op = CompiledOperator::AlwaysTrue};
                }
                else if constexpr (std::is_same_v<T, ComparisonConditionDefinition>)
                {
                    const ValueType valueType = resolveOperandType(axis, x.left);

                    return CompiledCondition{.op = internal::resolveOperator(x),
                                             .operandType = internal::ToCompiledValueType(valueType),
                                             .left = internal::makeCompiledOperand(x.left),
                                             .right = internal::makeCompiledOperand(x.right)};
                }
                else
                {
                    /**
                     * TODO: same concern as above - falls back to Equal silently in release builds.
                     */
                    assert(false && "Unhandled ConditionDefinition variant - validation should have caught this.");
                    return {};
                }
            },
            definition);
    }

    ValueType GetValueType(const ConditionValueType& definition)
    {
        return std::visit(
            [](auto&& x) -> ValueType
            {
                using T = std::remove_cvref_t<decltype(x)>;
                if constexpr (std::is_same_v<T, int>)
                {
                    return ValueType::Int;
                }
                else if constexpr (std::is_same_v<T, float>)
                {
                    return ValueType::Float;
                }
                else if constexpr (std::is_same_v<T, std::uint32_t>)
                {
                    return ValueType::UInt32;
                }
                else if constexpr (std::is_same_v<T, bool>)
                {
                    return ValueType::Bool;
                }
                else if constexpr (std::is_same_v<T, ddknd::math::uVec2>)
                {
                    return ValueType::UVec2;
                }
                else if constexpr (std::is_same_v<T, ddknd::math::uVec3>)
                {
                    return ValueType::UVec3;
                }
                else if constexpr(std::is_same_v<T, ddknd::math::Vec2f>)
                {
                    return ValueType::FVec2;
                }
                else if constexpr (std::is_same_v<T, ddknd::math::Vec3f>)
                {
                    return ValueType::FVec3;
                }
                else
                {
                    assert(false && "Unhandled Operator enum - validation should have caught this");
                    return ValueType::Float;
                }
            },
            definition);
    }

    CompiledOperator resolveOperator(const ComparisonConditionDefinition& definition)
    {
        switch (definition.op)
        {
        case Operator::Less:
            return CompiledOperator::Less;
        case Operator::LessEqual:
            return CompiledOperator::LessEqual;
        case Operator::Greater:
            return CompiledOperator::Greater;
        case Operator::GreaterEqual:
            return CompiledOperator::GreaterEqual;
        case Operator::Equal:
            return CompiledOperator::Equal;
        case Operator::NotEqual:
            return CompiledOperator::NotEqual;
        case Operator::None:
        {
            /**
             * TODO: assert() is stripped in release build. so reaching here would silently
             * fall back to a default value. This should only happen due to a missing check
             * in the validation layer or an incorrect call order.
             *
             * Consider a release-safe assertion mechanism(e.g. custom FSM_ASSERT macro) later.
             */
            assert(false && "Unhandled Operator enum - validation should have caught this.");
            return CompiledOperator::Equal;
        }
        default:
        {
            /**
             * TODO: same concern as above - falls back to Equal silently in release builds.
             */
            assert(false && "Unhandled Operator enum - validation should have caught this.");
            return CompiledOperator::Equal;
        }
        }
    }

    CompiledValueType resolveCompiledValueType(const ConditionValueType& definition)
    {
        return std::visit(
            [](auto& x) -> CompiledValueType
            {
                using T = std::remove_cvref_t<decltype(x)>;
                if constexpr (std::is_same_v<T, int>)
                {
                    return CompiledValueType::Int;
                }
                else if constexpr (std::is_same_v<T, float>)
                {
                    return CompiledValueType::Float;
                }
                else if constexpr (std::is_same_v<T, std::uint32_t>)
                {
                    return CompiledValueType::UInt32;
                }
                else if constexpr (std::is_same_v<T, bool>)
                {
                    return CompiledValueType::Bool;
                }
                else if constexpr (std::is_same_v<T, ddknd::math::uVec2>)
                {
                    return CompiledValueType::UVec2;
                }
                else if constexpr (std::is_same_v<T, ddknd::math::uVec3>)
                {
                    return CompiledValueType::UVec3;
                }
                else if constexpr(std::is_same_v<T, ddknd::math::Vec2f>)
                {
                    return CompiledValueType::FVec2;
                }
                else if constexpr (std::is_same_v<T, ddknd::math::Vec3f>)
                {
                    return CompiledValueType::FVec3;
                }
                else
                {
                    /**
                     * TODO: same concern as above - falls back to Equal silently in release builds.
                     */
                    assert(false && "Unhandled OperandDefinition variant - validation should have caught this.");
                    return CompiledValueType::Float;
                }
            },
            definition);
    }

    RawValue makeRawValueFromConditionValueType(const ConditionValueType& definition)
    {
        return std::visit(
            [](auto& x) -> RawValue
            {
                using T = std::remove_cvref_t<decltype(x)>;
                if constexpr (std::is_same_v<T, int>)
                {
                    return RawValue{.i = x};
                }
                else if constexpr (std::is_same_v<T, float>)
                {
                    return RawValue{.f = x};
                }
                else if constexpr (std::is_same_v<T, std::uint32_t>)
                {
                    return RawValue{.u = x};
                }
                else if constexpr (std::is_same_v<T, bool>)
                {
                    return RawValue{.b = x};
                }
                else if constexpr (std::is_same_v<T, ddknd::math::uVec2>)
                {
                    return RawValue{.uVec2 = x};
                }
                else if constexpr (std::is_same_v<T, ddknd::math::uVec3>)
                {
                    return RawValue{.uVec3 = x};
                }
                else if constexpr (std::is_same_v<T, ddknd::math::Vec2f>)
                {
                    return RawValue{.fVec2 = x};
                }
                else if constexpr (std::is_same_v<T, ddknd::math::Vec3f>)
                {
                    return RawValue{.fVec3 = x};
                }
                else
                {
                    /**
                     * TODO: same concern as above - falls back to Equal silently in release builds.
                     */
                    assert(false && "Unhandled ValueTypeDefinition variant - validation should have caught this.");
                    return {};
                }
            },
            definition);
    }

    CompiledOperand makeCompiledOperand(const OperandDefinition& definition)
    {
        return std::visit(
            [](auto& x) -> CompiledOperand
            {
                using T = std::remove_cvref_t<decltype(x)>;
                if constexpr (std::is_same_v<T, ConstantOperandDefinition>)
                {
                    CompiledOperand result;
                    result.isConstant = true;
                    result.constant.type = resolveCompiledValueType(x.value);
                    result.constant.raw = makeRawValueFromConditionValueType(x.value);
                    return result;
                }
                else if constexpr (std::is_same_v<T, ParameterOperandDefinition>)
                {
                    CompiledOperand result;
                    result.isConstant = false;
                    result.parameter = x.parameter;
                    return result;
                }
                else
                {
                    /**
                     * TODO: same concern as above - falls back to Equal silently in release builds.
                     */
                    assert(false && "Unhandled OperandDefinition variant - validation should have caught this.");
                    return {};
                }
            },
            definition);
    }

    CompiledValueType ToCompiledValueType(ValueType type)
    {
        switch (type)
        {
        case ValueType::Int:
            return CompiledValueType::Int;
        case ValueType::Float:
            return CompiledValueType::Float;
        case ValueType::UInt32:
            return CompiledValueType::UInt32;
        case ValueType::Bool:
            return CompiledValueType::Bool;
        case ValueType::UVec2:
            return CompiledValueType::UVec2;
        case ValueType::UVec3:
            return CompiledValueType::UVec3;
        case ValueType::FVec2:
            return CompiledValueType::FVec2;
        case ValueType::FVec3:
            return CompiledValueType::FVec3;
        
        }
        assert(false && "Unhandled ValueType enum - validation should have caught this.");
        return CompiledValueType::Float;
    }
} // namespace ddknd::fsm::internal