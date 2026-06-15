#include "game/system/state/game_character_state_system.h"

#include "game/component/state_component.h"
#include "game/component/IntentComponent.h"

#include <ddknd/math/math.h>

namespace app::system
{
    void PlayerLocomotionStateSystem::UpdateOne(app::component::PlayerLocomotionStateComponent& state,const app::component::MovementIntentComponent& moveIntent)
    {
        using State = app::component::PlayerLocomotionState;

        State next = State::Idle;

        if(moveIntent.active && ::ddknd::math::lengthSquared(moveIntent.direction) > ::ddknd::math::kEpsilonSq<float>)
        {
            next = State::Run_Forward;
        }

        state.changedThisFrame = state.current != next;
        state.previous = state.current;
        state.current = next;
    }
}