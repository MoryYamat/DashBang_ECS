#pragma once

#include <ddknd/ecs/entity/entity.h>
#include <ddknd/math/math.h>

namespace app::component
{
    struct HitboxComponent
    {
        ddknd::ecs::Entity owner;
        float lifetime = 0.1f;
        float elapsed = 0.0f;
    };

    struct SphereHitboxComponent
    {
        float radius = 1.0f;
    };

    struct HemisphereHitboxComponent
    {
        ddknd::math::Vec3f forward{0.0f, 0.0f, 1.0f};
        float radius = 1.0f;
    };

    struct HitboxSpawnRequest
    {
        ddknd::ecs::Entity owner;

        ddknd::math::Vec3f center;
        ddknd::math::Vec3f forward;

        float radius = 1.0f;
        float lifetime = 0.1f;

        enum class Shape
        {
            Sphere,
            hemiSphere
        } shape = Shape::hemiSphere;
    };

    struct HitboxSpawnRequestBuffer
    {
        std::vector<HitboxSpawnRequest> reqs_;
    };
}