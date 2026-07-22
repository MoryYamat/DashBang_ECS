#pragma once

#include <ddknd/fsm/fsm_id.h>

#include <cstdint>
#include <optional>
#include <variant>

/**
 * FSM Engine Frontend
 */
namespace ddknd::fsm
{
    enum class Operator : std::uint8_t
    {
        None,
        AlwaysTrue,
        Less,
        LessEqual,
        Greater,
        GreaterEqual,
        Equal,
        NotEqual
    };

    using ConditionValueType = std::variant<int, float, std::uint32_t, bool>;

    /**
    * If an operand is nullopt, its value is supplied by an external variable.
    *
    * Both operands may be std::nullopt, in which case both values are supplied externally.
    * Operator::AlwaysTrue does not require either operand.
    *
    * TODO Replace this simple representation with an AST(Abstract Syntax Tree) when more expressive conditions are required.
    */
    struct ConditionDefinition
    {
        Operator op = Operator::None;

        std::optional<ConditionValueType> leftValue;
        std::optional<ConditionValueType> rightValue;
    };

    /**
     * (Transition, Profile)->Condtion
     */
    struct TransitionConditionDefinition
    {
        TransitionID transition;
        ProfileID profile;

        ConditionDefinition condition;
        std::uint8_t priority = 0;
    };

} // namespace ddknd::fsm

/**
 * Questions:
 * - how to represent the condition values efficiently in runtime ?
 *   - which is better?: profile Only,
 *       or profile(value and its label) and condition(operation and type)
 */