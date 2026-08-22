#pragma once

#include <ddknd/component/component_fwd.h>

#include "game/component/game_component_fwd.h"

namespace fsm::MovementFSM
{
    struct MovementFSMStateComponent;
}

namespace app::system
{
    struct PlayerAnimationSystem
    {
        static void UpdateOne(::ddknd::component::AnimationPlaybackComponent& playback,
                              const component::PlayerLocomotionStateComponent& playerState,
                              const component::AttackStateComponent& attackState,
                              const component::CharacterAnimationClipsComponent& clips);

        static void UpdateOne(::ddknd::component::AnimationPlaybackComponent& playback,
                              const fsm::MovementFSM::MovementFSMStateComponent& playerState,
                              const component::AttackStateComponent& attackState,
                              const component::CharacterAnimationClipsComponent& clips);
    };
} // namespace app::system