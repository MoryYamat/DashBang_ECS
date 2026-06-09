#pragma once

#include <ddknd/input/input_fwd.h>
#include "game/component/game_component_fwd.h"

namespace app::system
{
    struct PlayerMovementIntentSystem
    {
        static void UpdateOne(
            app::component::RequestedMovementIntentComponent& movementIntent,
            const ::ddknd::input::ActionInputSystem& input);
    };
}