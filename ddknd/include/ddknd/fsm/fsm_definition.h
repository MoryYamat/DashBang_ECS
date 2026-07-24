#pragma once

#include <ddknd/fsm/fsm_id.h>

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

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
    struct ConditionDeclaration
    {
        Operator op = Operator::None;

        std::optional<ConditionValueType> leftValue;
        std::optional<ConditionValueType> rightValue;
    };

    struct StateDefinition
    {
        std::string debugName;
    };

    struct ConditionDefinition
    {
        std::string debugName;
    };

    struct ProfileDefinition
    {
        std::string debugName;
    };

    struct TransitionDefinition
    {
        std::string debugName;
        StateID from;
        StateID to;
    };

    struct TransitionConditionDefinition
    {
        TransitionID transition;
        ProfileID profile;
        ConditionDeclaration condition;
        std::uint8_t priority;
    };

    struct FSMDefinition
    {
        std::string debugName;
        std::vector<TransitionDefinition> transitions;
        std::vector<TransitionConditionDefinition> transitionConditions;
    };

    struct AxisDefinition
    {
        std::vector<FSMDefinition> fsms;
        std::vector<StateDefinition> states;
        std::vector<ConditionDefinition> conditions;
        std::vector<ProfileDefinition> profiles;
    };

} // namespace ddknd::fsm

/**
 * Questions:
 * - how to represent the condition values efficiently in runtime ?
 *   - which is better?: profile Only,
 *       or profile(value and its label) and condition(operation and type)
 */