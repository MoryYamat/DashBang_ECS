#include "game/system/state/game_character_state_system.h"

#include "game/component/IntentComponent.h"
#include "game/component/state_component.h"
#include "game/component/character_stats_component.h"

#include <ddknd/math/math.h>

namespace app::system
{
    void PlayerLocomotionStateSystem::UpdateOne(app::component::PlayerLocomotionStateComponent& state,
                                                const app::component::MovementIntentComponent& moveIntent)
    {
        using State = app::component::PlayerLocomotionState;

        State next = State::Idle;

        if (moveIntent.active && ::ddknd::math::lengthSquared(moveIntent.direction) > ::ddknd::math::kEpsilonSq<float>)
        {
            next = State::Run_Forward;
        }

        state.changedThisFrame = state.current != next;
        state.previous = state.current;
        state.current = next;
    }

    void PlayerAttackStateSystem::UpdateOne(app::component::AttackStateComponent& state,
                                            const app::component::AttackIntentComponent& attackIntent,
                                            const app::component::AttackDefComponent& def,
                                            const float dt)
    {
        using State = app::component::AttackState;

        state.previous = state.current;
        if(attackIntent.active && state.current == State::None)
        {
            state.current = State::Startup;
            state.elapsed = 0.0f;
            state.hitboxSpawned = false;
            return;
        }

        if(state.current == State::None)
        {
            return;
        }

        state.elapsed += dt;

        switch(state.current)
        {
            case State::Startup:
                if(state.elapsed >= def.timing.startupDuration)
                {
                    state.current = State::Active;
                    state.elapsed = 0.0f;
                }
                break;
            case State::Active:
                if(state.elapsed >= def.timing.activeDuration)
                {
                    state.current = State::Recovery;
                    state.elapsed = 0.0f;
                }
                break;
            case State::Recovery:
                if(state.elapsed >= def.timing.recoveryDuration)
                {
                    state.current = State::None;
                    state.elapsed = 0.0f;
                    state.hitboxSpawned = false;
                }
                break;

            case State::None:
            default:
                break;
        }
    }
} // namespace app::system