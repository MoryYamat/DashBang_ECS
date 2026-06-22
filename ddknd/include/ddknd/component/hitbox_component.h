#pragma once

#include <ddknd/ecs/entity/entity.h>
#include <ddknd/math/math.h>

namespace ddknd::component
{
    struct HitboxComponent
    {
        ddknd::ecs::Entity owner;
    };

    struct SphereHitboxComponent
    {
        float radius = 1.0f;
    };

    struct HemisphereHitboxComponent
    {
        float radius = 1.0f;
    };

    struct HurtboxComponent
    {
        // empty
    };

    struct SphereHurtboxComponent
    {
        float radius = 0.5f;
        ddknd::math::Vec3f localOffset{0.0f, 0.0f, 0.0f};
    };
}