#include "game/system/logic/game_animation_system.h"

#include <ddknd/component/gfx_component.h>

#include "game/component/animation_component.h"
#include "game/component/state_component.h"


#include "game/player/fsm/generated/movement_generated.h"

namespace app::system
{
    // Decide which clip to play
    void PlayerAnimationSystem::UpdateOne(::ddknd::component::AnimationPlaybackComponent& playback,
                                          const component::PlayerLocomotionStateComponent& playerState,
                                          const component::AttackStateComponent& attackState,
                                          const component::CharacterAnimationClipsComponent& clips)
    {
        using MoveState = app::component::PlayerLocomotionState;
        using AttackState = app::component::AttackState;
        auto nextClip = clips.idle;
        bool loop = true;
        float speed = 0.8f; // @TODO fix hardcoding

        if (attackState.current != AttackState::None)
        {
            nextClip = clips.attack;
            loop = false;
            speed = 1.2f; // @TODO fix hardcoding
            // std::cerr << "clip =" << nextClip.Value() << "\n";
        }
        else
        {
            switch (playerState.current)
            {
            case MoveState::Idle:
                nextClip = clips.idle;
                break;
            case MoveState::Run_Forward:
                nextClip = clips.runForward;
                break;
            case MoveState::Run_Backward:
                nextClip = clips.runBackward;
                break;
            case MoveState::Run_Left:
                nextClip = clips.runLeft;
                break;
            case MoveState::Run_Right:
                nextClip = clips.runRight;
                break;
            case MoveState::Run_Right_Foward_Diagonal:
                nextClip = clips.runRightFowardDiagonal;
                break;
            case MoveState::Run_Left_Foward_Diagonal:
                nextClip = clips.runLeftFowardDiagonal;
                break;
            case MoveState::Run_Right_Back_Diagonal:
                nextClip = clips.runRightBackDiagonal;
                break;
            case MoveState::Run_Left_Back_Diagonal:
                nextClip = clips.runLeftBackDiagonal;
                break;
            }
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

    void PlayerAnimationSystem::UpdateOne(::ddknd::component::AnimationPlaybackComponent& playback,
                          const fsm::MovementFSM::MovementFSMStateComponent& playerState,
                          const component::AttackStateComponent& attackState,
                          const component::CharacterAnimationClipsComponent& clips)
    {

        using MoveState = fsm::MovementFSM::MovementFSMState;
        using AttackState = app::component::AttackState;
        auto nextClip = clips.idle;
        bool loop = true;
        float speed = 0.8f; // @TODO fix hardcoding

        if (attackState.current != AttackState::None)
        {
            nextClip = clips.attack;
            loop = false;
            speed = 1.2f; // @TODO fix hardcoding
            // std::cerr << "clip =" << nextClip.Value() << "\n";
        }
        else
        {
            switch (playerState.instance.current)
            {
            case MoveState::Idle:
                nextClip = clips.idle;
                break;
            case MoveState::Run:
                nextClip = clips.runForward;
                break;
            }
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