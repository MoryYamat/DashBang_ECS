#pragma once

#include <ddknd/fsm/axis_definition.h>
#include <ddknd/fsm/axis_batch_compile_result.h>

#include <vector>

namespace ddknd::fsm
{
    class AxisBuildResult
    {

      public:
        AxisBuildResult(std::vector<AxisDefinition>&& def) : definitions_(std::move(def)) {}

        AxisBatchCompileResults CompileAllAxis();

      private:
        std::vector<AxisDefinition> definitions_;
    };
} // namespace ddknd::fsm