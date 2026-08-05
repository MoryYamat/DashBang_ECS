#pragma once

#include <ddknd/fsm/fsm_id.h>

namespace ddknd::fsm
{
    struct EvaluationResult
    {
        bool transitioned = false;
        StateID newState{};
    };
}