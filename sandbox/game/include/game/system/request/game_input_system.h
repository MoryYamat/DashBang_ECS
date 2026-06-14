#pragma once
#include "game/component/game_component_fwd.h"
#include <ddknd/component/component_fwd.h>
#include <ddknd/input/input_fwd.h>


namespace app::system
{
    struct PlayerMovementIntentSystem
    {
        static void UpdateOne(app::component::RequestedMovementIntentComponent& out,
                              const ::ddknd::input::ActionInputSystem& input);
    };

    struct PlayerCameraIntentSystem
    {
        static void UpdateOne(app::component::RequestedCameraIntentComponent& out,
                              const ::ddknd::input::ActionInputSystem& input);
    };
} // namespace app::system