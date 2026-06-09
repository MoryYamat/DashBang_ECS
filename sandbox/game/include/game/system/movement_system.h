#pragma once

#include <ddknd/component/component_fwd.h>
#include "game/component/game_component_fwd.h"

namespace app::system
{
    struct MovementSystem
    {
        static void UpdateOne(::ddknd::component::VelocityComponent& velocity,
                            const app::component::MovementIntentComponent& intent,
                            const app::component::CharacterMoveStatsComponent& stats);
    };
}