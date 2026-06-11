#include "game/system/logic/animation_system.h"

#include <ddknd/component/gfx_component.h>

#include "game/component/animation_component.h"
#include "game/component/state_component.h"

namespace app::system
{
    // Decide which clip to play
    void PlayerAnimationSystem::UpdateOne(::ddknd::component::AnimationPlaybackComponent& playback,
                                          const component::PlayerLocomotionStateComponent& playerState,
                                          const component::PlayerAnimationClipsComponent& clips)
    {
        using State = app::component::PlayerLocomotionState;

        auto nextClip = clips.idle;
        bool loop = true;
        float speed = 1.0f;

        switch (playerState.current)
        {
        case State::Idle:
            nextClip = clips.idle;
            break;
        case State::Run_Forward:
            nextClip = clips.runForward;
            break;
        case State::Run_Backward:
            nextClip = clips.runBackward;
            break;
        case State::Run_Left:
            nextClip = clips.runLeft;
            break;
        case State::Run_Right:
            nextClip = clips.runRight;
            break;
        case State::Run_Right_Foward_Diagonal:
            nextClip = clips.runRightFowardDiagonal;
            break;
        case State::Run_Left_Foward_Diagonal:
            nextClip = clips.runLeftFowardDiagonal;
            break;
        case State::Run_Right_Back_Diagonal:
            nextClip = clips.runRightBackDiagonal;
            break;
        case State::Run_Left_Back_Diagonal:
            nextClip = clips.runLeftBackDiagonal;
            break;
        }
        if (playback.state.clip == nextClip)
        {
            return;
        }

        playback.state.clip = nextClip;
        playback.state.time = 0.0f;
        playback.state.speed = speed;
        playback.state.loop = loop;
    }
} // namespace app::system