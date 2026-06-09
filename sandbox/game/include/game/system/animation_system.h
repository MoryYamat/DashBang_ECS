#pragma once

#include <ddknd/component/component_fwd.h>

namespace app::system
{
    struct PlayerAnimationSystem
    {
        static void UpdateOne(::ddknd::component::AnimationPlaybackComponent& playback/*, const PlayerStateComponent& playerState, const PlayerAnimationClipsComponent& clips*/);
    };
}