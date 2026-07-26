#pragma once

#include <ddknd/fsm/axis_definition.h>
#include <ddknd/fsm/fsm_id.h>

#include <string>
#include <string_view>
#include <unordered_map>

namespace ddknd::fsm
{
    struct StateBuildData
    {
        std::string name;
    };

    struct ParameterBuildData
    {
        ValueType type = ValueType::Float;
        std::string name;
    };

    struct ProfileBuildData
    {
        std::string name;
    };

    struct TransitionBuildData
    {
        std::string name;
        StateID from;
        StateID to;
    };

    struct TransitionConditionBuildData
    {
        TransitionID transition;
        ProfileID profile;
        ConditionDeclaration condition;
        std::uint8_t priority;
    };

    struct FSMBuildData
    {
        std::string name;

        std::unordered_map<std::string, TransitionID> transitionNameToId;
        std::vector<TransitionBuildData> transitions;
        std::vector<TransitionConditionBuildData> transitionConditions;
    };

    class FSMBuilder;

    /**
     * front-end for building axis definition.
     */
    class AxisBuilder
    {
      public:
        AxisBuilder() = default;
        AxisBuilder(const AxisBuilder&) = delete;
        AxisBuilder& operator=(const AxisBuilder&) = delete;
        AxisBuilder(AxisBuilder&&) = delete;
        AxisBuilder&& operator=(AxisBuilder&&) = delete;

        FSMID DeclareFSM(std::string_view fsmName);
        StateID DeclareState(std::string_view stateName);
        ParameterID DeclareParameter(ValueType type, std::string_view parameterName);
        ProfileID DeclareProfile(std::string_view profileName);

        FSMBuilder GetFSMBuilder(FSMID id);
        FSMBuildData& GetFSMBuildData(FSMID id);
        const FSMBuildData& GetFSMBuildData(FSMID id) const;

        bool IsValidFSMID(FSMID id) const;
        bool IsValidParameterID(ParameterID id) const;
        bool IsValidStateID(StateID id) const;
        bool IsValidProfileID(ProfileID id) const;

        AxisDefinition Build() &&;

      private:
        static FSMDefinition buildFSM(FSMBuildData&& source);
        static TransitionDefinition buildTransition(TransitionBuildData&& source);
        static ConditionDefinition buildCondition(ConditionDeclaration&& source);
        static OperandDefinition buildOperand(OperandDeclaration&& source);
        static TransitionConditionDefinition buildTransitionCondition(TransitionConditionBuildData&& source);

      private:
        std::string axisName_;

        // Axis Local IDs
        std::unordered_map<std::string, FSMID> fsmNameToId_;
        std::unordered_map<std::string, StateID> stateNameToId_;
        std::unordered_map<std::string, ParameterID> parameterNameToId_;
        std::unordered_map<std::string, ProfileID> profileNameToId_;

        std::vector<FSMBuildData> fsms_;
        std::vector<StateBuildData> states_;
        std::vector<ParameterBuildData> parameters_;
        std::vector<ProfileBuildData> profiles_;

        bool built_ = false;
    };

    class FSMBuilder
    {
      public:
        FSMBuilder(AxisBuilder& axis, FSMID id) : owner_(&axis), fsm_(id) {}

        TransitionID DeclareTransition(std::string_view name, StateID from, StateID to);
        void DefineTransitionCondition(TransitionID transitionId, ProfileID profileId,
                                       ConditionDeclaration conditionDeclaration, std::uint8_t priority);

      private:
        AxisBuilder* owner_ = nullptr;
        FSMID fsm_;
    };
} // namespace ddknd::fsm