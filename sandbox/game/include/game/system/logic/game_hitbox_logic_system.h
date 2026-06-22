#pragma once

#include <ddknd/ecs/ecs_fwd.h>

namespace app::system
{
    struct AttackHitboxSpawnSystem
    {
        static void Update(ddknd::ecs::World& world);
    };

    struct HitboxLifetimeSystem
    {
        static void Update(ddknd::ecs::World& world, const float dt);
    };
}