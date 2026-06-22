#pragma once

#include <ddknd/ecs/ecs_fwd.h>

namespace ddknd::system
{
    struct LifetimeSystem
    {
        static void Update(ddknd::ecs::World& world, const float dt);
    };
}