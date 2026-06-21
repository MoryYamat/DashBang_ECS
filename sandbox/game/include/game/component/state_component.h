#pragma once

namespace app::component
{
    // temporary
    enum class PlayerLocomotionState
    {
        Idle,
        Run_Forward,
        Run_Backward,
        Run_Right,
        Run_Left,
        Run_Right_Foward_Diagonal,
        Run_Left_Foward_Diagonal,
        Run_Right_Back_Diagonal,
        Run_Left_Back_Diagonal,
    };

    enum class AttackState
    {
        None,
        Startup,
        Active,
        Recovery
    };

    struct PlayerLocomotionStateComponent
    {
        PlayerLocomotionState current = PlayerLocomotionState::Idle;
        PlayerLocomotionState previous = PlayerLocomotionState::Idle;
        bool changedThisFrame = false;
    };

    struct AttackStateComponent
    {
        AttackState current = AttackState::None;
        AttackState previous = AttackState::None;

        float elapsed = 0.0f;

        bool hitboxSpawned = false;
    };
} // namespace app::component