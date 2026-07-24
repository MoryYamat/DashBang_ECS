#pragma once

#include <ddknd/core/StrongID.h>
#include <ddknd/fsm/fsm_tag.h>

namespace ddknd::fsm
{
    template<typename Tag>
    using ID = ddknd::core::StrongID<Tag>;

    using AxisID = ID<tag::AxisTag>;
    using FSMID = ID<tag::FSMTag>;
    using StateID = ID<tag::StateTag>;
    using ParameterID = ID<tag::ParameterTag>;
    using TransitionID = ID<tag::TransitionTag>;
    using ProfileID = ID<tag::ProfileTag>;
}