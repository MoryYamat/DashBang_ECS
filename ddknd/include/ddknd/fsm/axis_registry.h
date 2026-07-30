#pragma once

#include <ddknd/fsm/compile_result.h>
#include <ddknd/fsm/fsm_id.h>

#include <vector>

namespace ddknd::fsm
{
    /**
     * Hold all compiled axis data.
     */
    class AxisRegistry
    {
      public:
        void Set(const AxisID id, CompiledAxis&& axis);

        bool IsValidAxisID(AxisID id) const;
        const CompiledAxis& Get(AxisID id) const;

      private:
        std::vector<CompiledAxis> axes_;
    };
} // namespace ddknd::fsm