#pragma once

#include <ddknd/component/component_fwd.h>
#include "game/component/game_component_fwd.h"

namespace app::system
{
    struct MovementSystem
    {
        static void UpdateOne(::ddknd::component::VelocityComponent& velocity,
                            const app::component::MovementIntentComponent& intent,
                            const app::component::CharacterMoveStatsComponent& stats,
                            const app::component::CharacterControlModifierComponent& modifier);
    };

    struct AttackControlModifierSystem
    {
        static void UpdateOne(app::component::CharacterControlModifierComponent& out,
                            const app::component::AttackStateComponent& attack,
                            const app::component::AttackDefComponent& def);
    };
}