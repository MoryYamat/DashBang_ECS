#pragma once

#include <ddknd/component/component_fwd.h>

namespace ddknd::system
{
    class CameraMatricesSystem
    {
      public:
        static void UpdateOne(::ddknd::component::CameraMatricesComponent& matrices,
                              const ::ddknd::component::TransformComponent& transform,
                              const ::ddknd::component::CameraLookComponent& look,
                              const ::ddknd::component::CameraProjectionComponent& projection);
    };

    class CameraProjectionSystem
    {
      public:
        static void UpdateOne(component::CameraProjectionComponent& projection, const float aspect);
    };
} // namespace ddknd::system