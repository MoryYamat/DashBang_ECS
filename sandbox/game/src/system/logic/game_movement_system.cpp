#include "game/system/logic/game_movement_system.h"

#include <ddknd/component/movement_component.h>

#include "game/component/state_component.h"
#include "game/component/IntentComponent.h"
#include "game/component/character_stats_component.h"

namespace app::system
{
    void MovementSystem::UpdateOne(::ddknd::component::VelocityComponent& velocity,
                                   const app::component::MovementIntentComponent& intent,
                                   const app::component::CharacterMoveStatsComponent& stats,
                                   const app::component::CharacterControlModifierComponent& modifier)
    {
        if (!intent.active)
        {
            velocity.linear = {0.0f, 0.0f, 0.0f};
            return;
        }

        velocity.linear = intent.direction * stats.moveSpeed * modifier.moveSpeedMultiplier;
    }

    void AttackControlModifierSystem::UpdateOne(app::component::CharacterControlModifierComponent& out,
                                                const app::component::AttackStateComponent& attack,
                                                const app::component::AttackDefComponent& def)
    {
        out.moveSpeedMultiplier = 1.0f;
        out.lockFacing = false;

        if(attack.current == app::component::AttackState::None)
        {
            return;
        }

        out.moveSpeedMultiplier = def.control.moveSpeedMultiplier;
        out.lockFacing = def.control.lockFacing;
    }
} // namespace app::system