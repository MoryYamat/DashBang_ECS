#include <ddknd/fsm/builder.h>

#include <algorithm>
#include <cassert>
#include <string>
#include <type_traits>
#include <variant>

namespace ddknd::fsm
{
    FSMID AxisBuilder::DeclareFSM(std::string_view fsmName)
    {
        assert(!built_);
        auto name = std::string{fsmName};
        const auto it = fsmNameToId_.find(name);
        if (it == fsmNameToId_.end())
        {
            const auto size = fsms_.size();
            FSMID id = FSMID{static_cast<std::uint32_t>(size)};
            fsms_.push_back(FSMBuildData{.name = name});
            fsmNameToId_[name] = id;
            return id;
        }
        return it->second;
    }

    StateID AxisBuilder::DeclareState(std::string_view stateName)
    {
        assert(!built_);
        auto name = std::string{stateName};
        const auto it = stateNameToId_.find(name);
        if (it == stateNameToId_.end())
        {
            const auto size = states_.size();
            StateID id = StateID{static_cast<std::uint32_t>(size)};
            states_.push_back(StateBuildData{.name = name});
            stateNameToId_[name] = id;
            return id;
        }
        return it->second;
    }

    ParameterID AxisBuilder::DeclaraParameter(std::string_view parameterName)
    {
        assert(!built_);
        auto name = std::string{parameterName};
        const auto it = parameterNameToId_.find(name);
        if (it == parameterNameToId_.end())
        {
            const auto size = parameters_.size();
            ParameterID id = ParameterID{static_cast<std::uint32_t>(size)};
            parameters_.push_back(ParameterBuildData{.name = name});
            parameterNameToId_[name] = id;
            return id;
        }
        return it->second;
    }

    ProfileID AxisBuilder::DeclareProfile(std::string_view profileName)
    {
        assert(!built_);
        auto name = std::string{profileName};
        const auto it = profileNameToId_.find(name);
        if (it == profileNameToId_.end())
        {
            const auto size = profiles_.size();
            ProfileID id = ProfileID{static_cast<std::uint32_t>(size)};
            profiles_.push_back(ProfileBuildData{.name = name});
            profileNameToId_[name] = id;
            return id;
        }
        return it->second;
    }

    FSMBuilder AxisBuilder::GetFSMBuilder(FSMID id)
    {
        assert(id.IsValid());
        assert(static_cast<std::size_t>(id.Value()) < fsms_.size());

        return FSMBuilder{*this, id};
    }

    FSMBuildData& AxisBuilder::GetFSMBuildData(FSMID id)
    {
        assert(IsValidFSMID(id));
        return fsms_[static_cast<std::size_t>(id.Value())];
    }

    const FSMBuildData& AxisBuilder::GetFSMBuildData(FSMID id) const
    {
        assert(IsValidFSMID(id));
        return fsms_[static_cast<std::size_t>(id.Value())];
    }

    bool AxisBuilder::IsValidFSMID(FSMID id) const
    {
        if (!id.IsValid())
        {
            return false;
        }

        if (static_cast<std::size_t>(id.Value()) > fsms_.size())
        {
            return false;
        }

        return true;
    }

    bool AxisBuilder::IsValidParameterID(ParameterID id) const
    {
        if (!id.IsValid())
        {
            return false;
        }

        if (static_cast<std::size_t>(id.Value()) > parameters_.size())
        {
            return false;
        }

        return true;
    }

    bool AxisBuilder::IsValidStateID(StateID id) const
    {
        if (!id.IsValid())
        {
            return false;
        }

        if (static_cast<std::size_t>(id.Value()) > states_.size())
        {
            return false;
        }

        return true;
    }

    bool AxisBuilder::IsValidProfileID(ProfileID id) const
    {
        if (!id.IsValid())
        {
            return false;
        }

        if (static_cast<std::size_t>(id.Value()) > profiles_.size())
        {
            return false;
        }

        return true;
    }

    AxisDefinition AxisBuilder::Build() &&
    {
        assert(!built_ && "AxisBuilder::Build() has already been built and cannot be reused.");
        built_ = true;

        AxisDefinition result{};

        result.states.reserve(states_.size());
        for (auto& state : states_)
        {
            result.states.emplace_back(std::move(state.name));
        }

        result.parameters.reserve(parameters_.size());
        for (auto& parameter : parameters_)
        {
            result.parameters.emplace_back(std::move(parameter.name));
        }

        result.profiles.reserve(profiles_.size());
        for (auto& profile : profiles_)
        {
            result.profiles.emplace_back(std::move(profile.name));
        }

        result.fsms.reserve(fsms_.size());
        for (auto& fsm : fsms_)
        {
            result.fsms.push_back(buildFSM(std::move(fsm)));
        }

        return result;
    }

    FSMDefinition AxisBuilder::buildFSM(FSMBuildData&& source)
    {
        FSMDefinition result{};

        result.debugName = std::move(source.name);

        result.transitions.reserve(source.transitions.size());
        for (auto& transition : source.transitions)
        {
            result.transitions.emplace_back(buildTransition(std::move(transition)));
        }

        result.transitionConditions.reserve(source.transitionConditions.size());
        for (auto& transitionCondition : source.transitionConditions)
        {
            result.transitionConditions.emplace_back(buildTransitionCondition(std::move(transitionCondition)));
        }

        return result;
    }

    TransitionDefinition AxisBuilder::buildTransition(TransitionBuildData&& source)
    {
        return TransitionDefinition{.debugName = std::move(source.name), .from = source.from, .to = source.to};
    }

    ConditionDefinition AxisBuilder::buildCondition(ConditionDeclaration&& source)
    {
        return std::visit(
            [](auto&& condition) -> ConditionDefinition
            {
                using T = std::remove_cvref_t<decltype(condition)>;

                if constexpr (std::is_same_v<T, AlwaysTrueConditionDeclaration>)
                {
                    return AlwaysTrueConditionDefinition{};
                }
                else
                {
                    return ComparisonConditionDefinition{.op = condition.op,
                                                         .left = buildOperand(std::move(condition.left)),
                                                         .right = buildOperand(std::move(condition.right))};
                }
            },
            std::move(source));
    }

    OperandDefinition AxisBuilder::buildOperand(OperandDeclaration&& source)
    {
        return std::visit(
            [](auto&& operand) -> OperandDefinition
            {
                using T = std::remove_cvref_t<decltype(operand)>;

                if constexpr (std::is_same_v<T, ConstantOperandDeclaration>)
                {
                    return ConstantOperandDefinition{.value = std::move(operand.value)};
                }
                else
                {
                    return ParameterOperandDefinition{.parameter = operand.parameter};
                }
            },
            std::move(source));
    }

    TransitionConditionDefinition AxisBuilder::buildTransitionCondition(TransitionConditionBuildData&& source)
    {
        return TransitionConditionDefinition{.transition = source.transition,
                                             .profile = source.profile,
                                             .condition = std::move(buildCondition(std::move(source.condition))),
                                             .priority = source.priority};
    }

    TransitionID FSMBuilder::DeclareTransition(std::string_view name, StateID from, StateID to)
    {
        assert(owner_);
        assert(owner_->IsValidFSMID(fsm_));
        assert(owner_->IsValidStateID(from));
        assert(owner_->IsValidStateID(to));

        auto& fsmData = owner_->GetFSMBuildData(fsm_);

        const auto it = fsmData.transitionNameToId.find(std::string{name});
        if (it == fsmData.transitionNameToId.end())
        {
            const auto size = fsmData.transitions.size();
            TransitionID id = TransitionID{static_cast<std::uint32_t>(size)};
            fsmData.transitions.push_back({.name = std::string{name}, .from = from, .to = to});
            fsmData.transitionNameToId[std::string{name}] = id;
            return id;
        }

        /**
         * If the name already exists, the state transition definitions must be identical.
         */
        const TransitionID existingID = it->second;
        const auto index = static_cast<std::size_t>(existingID.Value());
        assert(index < fsmData.transitions.size());

        const auto& existing = fsmData.transitions[index];
        assert(existing.from == from);
        assert(existing.to == to);

        return existingID;
    }

    void FSMBuilder::DefineTransitionCondition(TransitionID transitionId, ProfileID profileId,
                                               ConditionDeclaration conditionDeclaration, std::uint8_t priority)
    {
        assert(owner_);
        assert(owner_->IsValidFSMID(fsm_));
        assert(transitionId.IsValid());
        assert(profileId.IsValid());

        auto& fsmData = owner_->GetFSMBuildData(fsm_);

        assert(fsmData.transitions.size() > static_cast<std::size_t>(transitionId.Value()));
        assert(owner_->IsValidProfileID(profileId));

        const auto duplicated =
            std::ranges::find_if(fsmData.transitionConditions, [&](const TransitionConditionBuildData& definition)
                                 { return definition.transition == transitionId && definition.profile == profileId; });

        assert(duplicated == fsmData.transitionConditions.end());

        fsmData.transitionConditions.push_back(
            TransitionConditionBuildData{.transition = transitionId,
                                         .profile = profileId,
                                         .condition = std::move(conditionDeclaration),
                                         .priority = priority});
    }

} // namespace ddknd::fsm