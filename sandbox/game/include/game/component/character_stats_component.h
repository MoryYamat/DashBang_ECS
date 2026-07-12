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


    // ==================================================
    // Until the FSM-Engine is implemented, a `Def`
    //  is defined here as a temporary implementation.

    // @TODO Fix hardcoded values.
    struct AttackTimingDef
    {

        float startupDuration = 0.70f;  
        float activeDuration = 0.35f;   
        float recoveryDuration = 0.45f; 
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