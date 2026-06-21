#pragma once

namespace app::component
{
    struct CharacterMoveStatsComponent
    {
        float moveSpeed = 5.0f;
    };
    
    struct CharacterControlModifierComponent
    {
        float moveSpeedMultiplier = 1.0f;
        bool lockFacing = false;
    };

    struct AttackTimingDef
    {
        float startupDuration = 0.80f;  // 50f
        float activeDuration = 0.30f;   // 20f
        float recoveryDuration = 0.45f; // 30f
    };

    struct AttackControlDef
    {
        float moveSpeedMultiplier = 0.0f;
        bool lockFacing = true;
    };

    struct AttackDefComponent
    {
        AttackTimingDef timing{};
        AttackControlDef control;
    };
} // namespace app::component