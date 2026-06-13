#include "game/system/logic/game_movement_system.h"

#include <ddknd/component/movement_component.h>

#include "game/component/IntentComponent.h"
#include "game/component/character_stats_component.h"

namespace app::system
{
    void MovementSystem::UpdateOne(::ddknd::component::VelocityComponent& velocity,
                                         const app::component::MovementIntentComponent& intent,
                                         const app::component::CharacterMoveStatsComponent& stats)
    {
        if(!intent.active)
        {
            velocity.linear = {0.0f, 0.0f, 0.0f};
            return;
        }

        velocity.linear = intent.direction * stats.moveSpeed;
    }
} // namespace app::system