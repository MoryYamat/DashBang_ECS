#pragma once

#include <ddknd/ecs/ecs_fwd.h>

namespace app::system
{
    struct AttackHitboxSpawnSystem
    {
        static void Update(ddknd::ecs::World& world);
    };



    struct HitboxCollisionSystem
    {
        static void Update(ddknd::ecs::World& world);
    };
} // namespace app::system