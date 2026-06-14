#pragma once

namespace app::component
{
    // requested intent
    struct RequestedMovementIntentComponent;
    struct CameraDesiredPoseComponent;
    struct CameraFollowTargetComponent;
    struct CameraOrbitComponent;
    struct RequestedCameraIntentComponent;
    struct CameraControllerSettingsComponent;

    // instanced data
    struct CharacterMoveStatsComponent;
    struct PlayerControllerComponent; 
    struct MovementIntentComponent;

    // state
    struct PlayerLocomotionStateComponent;

    // animation 
    struct PlayerAnimationClipsComponent;
}