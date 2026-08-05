#pragma once
#include <ddknd/fsm/axis_build_result.h>
#include <ddknd/fsm/axis_definition.h>
#include <ddknd/fsm/builder.h>
#include <ddknd/fsm/fsm_id.h>

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace ddknd::fsm
{
    struct AxisBuildHandle
    {
      AxisID id;
      AxisBuilder& builder;
    };

    class AxisBuilderRegistry
    {
      public:
        AxisBuildHandle GetOrCreateAxis(std::string_view axisName);

        // index = AxisID.Value()
        AxisBuildResult BuildAllAxis();

      private:
        std::vector<std::unique_ptr<AxisBuilder>> axisBuilders_;
        std::unordered_map<std::string, AxisID> nameToId_;
    };
} // namespace ddknd::fsm