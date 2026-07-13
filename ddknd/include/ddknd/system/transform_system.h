#pragma once

#include "ddknd/component/component_fwd.h"

namespace ddknd::system
{
    class TransformSystem
    {
      public:
        static void UpdateOne(component::TransformComponent& transform);
    };
} // namespace ddknd::system