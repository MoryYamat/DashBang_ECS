#pragma once

#include <ddknd/fsm/fsm_id.h>

#include <cstdint>
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
        Less,
        LessEqual,
        Greater,
        GreaterEqual,
        Equal,
        NotEqual
    };



    using ConditionValueType = std::variant<int, float, std::uint32_t, bool>;

    struct ConstantOperandDeclaration
    {
        ConditionValueType value;
    };

    struct ParameterOperandDeclaration
    {
        ParameterID parameter;
    };

    using OperandDeclaration = std::variant<ConstantOperandDeclaration, ParameterOperandDeclaration>;

    struct AlwaysTrueConditionDeclaration{};

    /**
    * A condition operand is either:
    * - a constant value embedded in the declaration, or
    * - a runtime parameter identified by ParameterID.
    *
    * TODO Replace this simple representation with an AST(Abstract Syntax Tree) when more expressive conditions are required.
    */
    struct ComparisonConditionDeclaration
    {
        Operator op = Operator::None;

        OperandDeclaration left;
        OperandDeclaration right;
    };

    using ConditionDeclaration = std::variant<AlwaysTrueConditionDeclaration, ComparisonConditionDeclaration>;

    struct StateDefinition
    {
        std::string debugName;
    };

    struct ParameterDefinition
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

    struct ConstantOperandDefinition
    {
        ConditionValueType value;
    };

    struct ParameterOperandDefinition
    {
        ParameterID parameter;
    };

    using OperandDefinition = std::variant<ConstantOperandDefinition, ParameterOperandDefinition>;

    struct AlwaysTrueConditionDefinition{};

    struct ComparisonConditionDefinition
    {
        Operator op = Operator::None;
        OperandDefinition left;
        OperandDefinition right;
    };

    using ConditionDefinition = std::variant<AlwaysTrueConditionDefinition, ComparisonConditionDefinition>;

    struct TransitionConditionDefinition
    {
        TransitionID transition;
        ProfileID profile;
        ConditionDefinition condition;
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
        std::vector<ParameterDefinition> parameters;
        std::vector<ProfileDefinition> profiles;
    };

} // namespace ddknd::fsm

/**
 * Questions:
 * - how to represent the condition values efficiently in runtime ?
 *   - which is better?: profile Only,
 *       or profile(value and its label) and condition(operation and type)
 */