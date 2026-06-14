#pragma once

#include "game/component/game_component_fwd.h"
#include <ddknd/component/component_fwd.h>
#include <ddknd/ecs/ecs_fwd.h>

namespace app::system
{
    struct MovementIntentResolveSystem
    {
        static void UpdateOne(app::component::MovementIntentComponent& out,
                              const app::component::RequestedMovementIntentComponent& request,
                              const ddknd::component::CameraLookComponent& cameraLook);
    };

    struct CameraIntentResolveSystem
    {
        static void UpdateOne(app::component::CameraOrbitComponent& orbit,
                              const app::component::RequestedCameraIntentComponent& request);
    };
} // namespace app::system