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
    struct PlayerControllerComponent; 

    // Requested Intent and Intent
    struct CharacterMoveStatsComponent;
    struct MovementIntentComponent;
    
    struct AttackDefComponent;
    struct RequestedAttackIntentComponent;
    struct AttackIntentComponent;
    struct CharacterControlModifierComponent;

    // state
    struct PlayerLocomotionStateComponent;
    struct AttackStateComponent;
    // animation 
    struct PlayerAnimationClipsComponent;
}