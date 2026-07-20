#pragma once

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
    /**
     * Axis is the vocabulary set of FSM.
     */
    struct AxisDefinition
    {
        std::string axisName;

        std::vector<std::string> fsmNames;
        std::vector<std::string> stateNames;
        std::vector<std::string> conditionNames;
        std::vector<std::string> profileNames;
    };

    enum class Operator : std::uint8_t
    {
        None,
        Always,
        Less,
        LessEqual,
        Greater,
        GreaterEqual,
        Equal,
        NotEqual
    };

    using ConditionValueType = std::variant<int, float, std::uint32_t, bool>;

    struct ConditionDefinition
    {
        Operator op = Operator::None;
        std::optional<ConditionValueType> leftValue;
        std::optional<ConditionValueType> rightValue;
    };

    struct TransitionDefinition
    {
        std::string transitionName;
        std::string fromState;
        std::string toState;
    };

    /**
     * (Transition, Profile)->Condtion
     */
    struct TransitionConditionDefinition
    {
        std::string transitionName;
        std::string profileName;

        ConditionDefinition condition;
        std::uint8_t priority = 0;
    };

    struct FSMDefinition
    {
        std::string fsmName;
        std::vector<TransitionDefinition> transitions;
        std::vector<TransitionConditionDefinition> transitionConditions;
    };

    struct AxsiRegistryEntry
    {
        AxisDefinition axis;
        std::vector<FSMDefinition> fsms;
    };

    class FSMDefinitionRegistry
    {
      public:
        void AddAxisEntry(AxsiRegistryEntry axis)
        {
            axisEntries_.push_back(axis);
        }

      private:
        std::vector<AxsiRegistryEntry> axisEntries_;
    };
} // namespace ddknd::fsm

/**
 * Questions:
 * - how to represent the condition values efficiently in runtime ?
 *   - which is better?: profile Only,
 *       or profile(value and its label) and condition(operation and type)
 */

 /**
 * SymbolTable (a class for mapping between user defined symbols and constructed canonical IDs)
 * This resolves the user runtime id data by symbol(string) after initialization.
 */