#pragma once

namespace app::component
{
    // requested intent
    struct RequestedMovementIntentComponent;
    struct CameraDesiredPoseComponent;
    struct CameraFollowTargetComponent;
    struct CameraOrbitComponent;

    // instanced data
    struct CharacterMoveStatsComponent;
    struct PlayerControllerComponent; 
    struct MovementIntentComponent;

    // state
    struct PlayerLocomotionStateComponent;

    // animation 
    struct PlayerAnimationClipsComponent;
}