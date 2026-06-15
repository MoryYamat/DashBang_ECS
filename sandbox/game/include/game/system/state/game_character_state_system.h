#pragma once

#include "game/component/game_component_fwd.h"

namespace app::system
{
    struct PlayerLocomotionStateSystem
    {
        static void UpdateOne(app::component::PlayerLocomotionStateComponent& state,const app::component::MovementIntentComponent& moveIntent);
    };
}