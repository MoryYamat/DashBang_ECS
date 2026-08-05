#pragma once

#include <ddknd/fsm/compile_result.h>
#include <ddknd/fsm/evaluation_result.h>
#include <ddknd/fsm/fsm_id.h>
#include <ddknd/fsm/parameter_set.h>

namespace ddknd::fsm
{
    EvaluationResult EvaluateFSM(const CompiledFSM& fsm, StateID currentState, ProfileID currentProfile,
                                 const ParameterSet& parameters);
}// namespace ddknd::fsm