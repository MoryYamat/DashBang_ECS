#include <ddknd/fsm/builder.h>

#include <algorithm>
#include <cassert>
#include <string>

namespace ddknd::fsm
{
    FSMID AxisBuilder::DeclareFSM(std::string_view fsmName)
    {
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

    ConditionID AxisBuilder::DeclareCondition(std::string_view stateName)
    {
        auto name = std::string{stateName};
        const auto it = conditionNameToId_.find(name);
        if (it == conditionNameToId_.end())
        {
            const auto size = conditions_.size();
            ConditionID id = ConditionID{static_cast<std::uint32_t>(size)};
            conditions_.push_back(ConditionBuildData{.name = name});
            conditionNameToId_[name] = id;
            return id;
        }
        return it->second;
    }

    ProfileID AxisBuilder::DeclareProfile(std::string_view profileName)
    {
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

        return FSMBuilder{this, id};
    }

    bool AxisBuilder::IsValidFSMID(FSMID id)
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

    bool AxisBuilder::IsValidStateID(StateID id)
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

    bool AxisBuilder::IsValidConditionID(ConditionID id)
    {
        if (!id.IsValid())
        {
            return false;
        }

        if (static_cast<std::size_t>(id.Value()) > conditions_.size())
        {
            return false;
        }

        return true;
    }

    bool AxisBuilder::IsValidProfileID(ProfileID id)
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

    TransitionID FSMBuilder::DeclareTransition(std::string_view name, StateID from, StateID to)
    {
        assert(owner_);
        assert(owner_->IsValidStateID(from));
        assert(owner_->IsValidStateID(to));

        const auto it = transitionNameToId_.find(std::string{name});
        if (it == transitionNameToId_.end())
        {
            const auto size = transitions_.size();
            TransitionID id = TransitionID{static_cast<std::uint32_t>(size)};
            transitions_.push_back({
                .name = std::string{name},
                .from = from,
                .to = to
            });
            transitionNameToId_[std::string{name}] = id;
            return id;
        }

        /**
        * If the name already exists, the state transition definitions must be identical.
        */
        const TransitionID existingID = it->second;
        const auto index = static_cast<std::size_t>(existingID.Value());
        assert(index < transitions_.size());
        
        const auto& existing = transitions_[index];
        assert(existing.from == from);
        assert(existing.to == to);

        return existingID;
    }

    void FSMBuilder::DefineTransitionCondition(TransitionID transitionId, ProfileID profileId,
                                               ConditionDefinition conditionDefinition, std::uint8_t priority)
    {
        assert(owner_);
        assert(transitionId.IsValid());
        assert(profileId.IsValid());
        assert(transitions_.size() > static_cast<std::size_t>(transitionId.Value()));
        assert(owner_->IsValidProfileID(profileId));

        const auto duplicated =
            std::ranges::find_if(transitionConditions_, [&](const TransitionConditionBuildData& definition)
                                 { return definition.transition == transitionId && definition.profile == profileId; });

        assert(duplicated == transitionConditions_.end());

        transitionConditions_.push_back(
            TransitionConditionBuildData
            {
                .transition = transitionId,
                .profile = profileId,
                .condition = std::move(conditionDefinition)
            });
    }

} // namespace ddknd::fsm