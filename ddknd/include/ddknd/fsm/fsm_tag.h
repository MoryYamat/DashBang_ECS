#pragma once

#include <ddknd/core/StrongID.h>

namespace ddknd::fsm
{
    template<typename Tag>
    using ID = ddknd::core::StrongID<Tag>;

    struct AxisTag{};
    struct FSMTag{};
    struct StateTag{};
    struct ConditionTag{};
    struct ProfileTag{};


    using AxisID = ID<AxisTag>;
    using FSMID = ID<FSMTag>;
    using StateID = ID<StateTag>;
    using ConditionID = ID<ConditionTag>;
    using ProfileID = ID<ProfileTag>;
}