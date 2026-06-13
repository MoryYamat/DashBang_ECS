#pragma once

#include <ddknd/component/component_fwd.h>
#include "game/component/game_component_fwd.h"
#include <ddknd/ecs/ecs_fwd.h>


namespace app::system
{
    struct MovementIntentResolveSystem
    {
        static void UpdateOne(app::component::MovementIntentComponent& out,
                              const app::component::RequestedMovementIntentComponent& request,
                              const ddknd::component::CameraLookComponent& cameraLook);
    };
} // namespace app::system