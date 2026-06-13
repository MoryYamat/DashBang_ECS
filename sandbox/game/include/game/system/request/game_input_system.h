#pragma once
#include <ddknd/component/component_fwd.h>
#include <ddknd/input/input_fwd.h>
#include "game/component/game_component_fwd.h"

namespace app::system
{
    struct PlayerMovementIntentSystem
    {
        static void UpdateOne(
            app::component::RequestedMovementIntentComponent& out,
            const ::ddknd::input::ActionInputSystem& input);
    };
}