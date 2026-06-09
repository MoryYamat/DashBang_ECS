#pragma once
#include <ddknd/component/component_fwd.h>

namespace ddknd::component
{
    struct DebugCameraControllerComponent;
}

namespace ddknd::system
{
    class DebugCameraSystem
    {
      public:
        static void UpdateDebugCamera(const component::TransformComponent& transform,
                                      component::DebugCameraControllerComponent& camera);
    };
} // namespace ddknd::system