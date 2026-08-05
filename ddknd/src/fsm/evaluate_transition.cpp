#include <ddknd/fsm/evaluate_transition.h>

#include <cstddef>
#include <cstdint>

namespace ddknd::fsm
{
    RawValue ResolveOperand(const CompiledOperand& operand, const ParameterSet& parameters)
    {
        using namespace ddknd::fsm;
        if (operand.isConstant)
        {
            return operand.constant.raw;
        }
        return parameters.GetParameter(operand.parameter);
    }

    template <typename T>
    bool CompareRaw(CompiledOperator op, T left, T right)
    {
        switch (op)
        {
        case CompiledOperator::AlwaysTrue:
            return true;
        case CompiledOperator::Greater:
            return left > right;
        case CompiledOperator::GreaterEqual:
            return left >= right;
        case CompiledOperator::Less:
            return left < right;
        case CompiledOperator::LessEqual:
            return left <= right;
        case CompiledOperator::Equal:
            return left == right;
        case CompiledOperator::NotEqual:
            return left != right;
        }

        return false;
    }

    bool Compare(CompiledOperator op, CompiledValueType type, const RawValue& left, const RawValue& right)
    {
        switch (type)
        {
        case CompiledValueType::Int:
            return CompareRaw(op, left.i, right.i);
        case CompiledValueType::Float:
            return CompareRaw(op, left.f, right.f);
        case CompiledValueType::UInt32:
            return CompareRaw(op, left.u, right.u);
        case CompiledValueType::Bool:
            return CompareRaw(op, left.b, right.b);
        }

        return false;
    }

    bool EvaluateCondition(const CompiledCondition& condition, const ParameterSet& parameters)
    {
        if (condition.op == CompiledOperator::AlwaysTrue)
        {
            return true;
        }

        const RawValue left = ResolveOperand(condition.left, parameters);
        const RawValue right = ResolveOperand(condition.right, parameters);

        return Compare(condition.op, condition.operandType, left, right);
    }

    EvaluationResult EvaluateFSM(const CompiledFSM& fsm, StateID currentState, ProfileID currentProfile,
                                 const ParameterSet& parameters)
    {
        EvaluationResult result{};
        result.newState = currentState;

        const auto stateValue = static_cast<std::size_t>(currentState.Value());
        if (stateValue >= fsm.stateToTransitionRange.size())
        {
            return result;
        }

        const auto& range = fsm.stateToTransitionRange[stateValue];

        bool found = false;
        std::uint8_t bestPriority = 0;
        StateID bestTo{};

        for (auto i = 0; i < range.count; ++i)
        {
            const TransitionID transitionId = fsm.transitionByFrom[range.begin + i];
            const auto index = static_cast<std::size_t>(transitionId.Value() * fsm.ProfileCount +
                                                        static_cast<std::size_t>(currentProfile.Value()));

            const CompiledTransitionCondition& tc = fsm.transitionConditions[index];

            if (EvaluateCondition(tc.condition, parameters))
            {
                if (!found || tc.priority > bestPriority)
                {
                    found = true;
                    bestPriority = tc.priority;
                    bestTo = tc.to;
                }
            }
        }

        if (found)
        {
            result.transitioned = true;
            result.newState = bestTo;
        }

        return result;
    }
} // namespace ddknd::fsm