#pragma once

#include <ddknd/fsm/fsm_id.h>

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include <ddknd/math/math.h>

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

    enum class ValueType : std::uint8_t
    {
        Int,
        Float,
        UInt32,
        Bool,
        UVec2,
        UVec3,
        FVec2,
        FVec3
    };

    using ConditionValueType = std::variant<int, float, std::uint32_t, bool, ddknd::math::uVec2, ddknd::math::uVec3, ddknd::math::Vec2f, ddknd::math::Vec3f>;

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
        ValueType type = ValueType::Float;
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