#pragma once

#include "game/component/game_component_fwd.h"
#include <ddknd/component/component_fwd.h>


namespace app::system
{
    struct CharacterFacingSystem
    {
        static void UpdateOne(::ddknd::component::TransformComponent& transform,
                         const app::component::MovementIntentComponent& moveIntent);
    };
} // namespace app::system