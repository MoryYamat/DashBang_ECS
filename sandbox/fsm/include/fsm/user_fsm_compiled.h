#pragma once

#include <ddknd/fsm/fsm_id.h>

namespace fsm::uesr
{
    /**
    * user have to create these kind of data structure
    */
    struct TestFSMCompiled
    {
        ddknd::fsm::AxisID axis;
        ddknd::fsm::FSMID testFSM;

        ddknd::fsm::StateID firstState;
        ddknd::fsm::StateID secondState;
        ddknd::fsm::StateID thirdState;

        ddknd::fsm::ProfileID firstSecond;
        ddknd::fsm::ProfileID secondThird;
        ddknd::fsm::ProfileID thirdFirst;

        ddknd::fsm::TransitionID firstTransition;
        ddknd::fsm::TransitionID secondTransition;
        ddknd::fsm::TransitionID thirdTransition;

        ddknd::fsm::ConditionID normalTimer;
        ddknd::fsm::ConditionID AlwayTrue;
    };
}