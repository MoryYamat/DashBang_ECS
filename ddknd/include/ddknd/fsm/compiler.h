#pragma once

#include <ddknd/fsm/compile_result.h>
#include <ddknd/fsm/axis_definition.h>

namespace ddknd::fsm
{
    struct AxisCompiler
    {
        static AxisCompileResult Compile(AxisDefinition& definition);
    };
} // namespace ddknd::fsm