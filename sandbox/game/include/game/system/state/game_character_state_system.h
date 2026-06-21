#pragma once

#include "game/component/game_component_fwd.h"

namespace app::system
{
    struct PlayerLocomotionStateSystem
    {
        static void UpdateOne(app::component::PlayerLocomotionStateComponent& state,const app::component::MovementIntentComponent& moveIntent);
    };

    struct PlayerAttackStateSystem
    {
        static void UpdateOne(app::component::AttackStateComponent& state, const app::component::AttackIntentComponent& attackIntent, const app::component::AttackDefComponent& def, const float dt);
    };
}