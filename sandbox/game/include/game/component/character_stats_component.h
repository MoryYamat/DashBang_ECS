#pragma once

#include <ddknd/math/math.h>

namespace app::component
{
    struct CharacterMoveStatsComponent
    {
        float moveSpeed = 1.0f;
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

    struct AttackHitboxDef
    {
        float radius = 1.0f;
        float lifetime =0.1f;

        ddknd::math::Vec3f localOffset;

        bool useforwardHemisphere = true;
    };

    struct AttackDefComponent
    {
        AttackTimingDef timing{};
        AttackControlDef control;
        AttackHitboxDef hitbox;
    };
} // namespace app::component